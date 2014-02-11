// Created by Davis E. King on Feb 11, 2014
#include <mitie.h>

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <assert.h>
#include <mitie/named_entity_extractor.h>
#include <mitie/unigram_tokenizer.h>

using namespace mitie;

extern "C"
{

// ----------------------------------------------------------------------------------------

    enum mitie_object_type
    {
        MITIE_NOT_A_MITIE_OBJECT = 0,
        MITIE_NAMED_ENTITY_EXTRACTOR = 1234,
        MITIE_NAMED_ENTITY_DETECTIONS
    };

    struct mitie_named_entity_detections_impl
    {
        std::vector<std::pair<unsigned long, unsigned long> > ranges;
        std::vector<unsigned long> predicted_labels;
        std::vector<std::string> tags;
    };

    // The reason for the impl objects is to ensure that the types in the public interface
    // are POD types.  This will let us have one single free function (mitie_free()) which
    // just looks at the headers for each object and then decides how to free them.  
    struct mitie_named_entity_extractor
    {
        mitie_object_type type;
        named_entity_extractor* impl;
    };

    struct mitie_named_entity_detections
    {
        mitie_object_type type;
        mitie_named_entity_detections_impl* impl;
    };

    void mitie_free (
        void* object 
    )
    {
        if (object == 0)
            return;

        switch(*((mitie_object_type*)object))
        {
            case MITIE_NAMED_ENTITY_EXTRACTOR:
                {
                    mitie_named_entity_extractor* obj;
                    obj = static_cast<mitie_named_entity_extractor*>(object);
                    // set this because it can help detect multiple frees
                    obj->type = MITIE_NOT_A_MITIE_OBJECT;
                    delete obj->impl;
                    delete obj;
                } break;
            case MITIE_NAMED_ENTITY_DETECTIONS:
                {
                    mitie_named_entity_detections* obj;
                    obj = static_cast<mitie_named_entity_detections*>(object);
                    // set this because it can help detect multiple frees
                    obj->type = MITIE_NOT_A_MITIE_OBJECT;
                    delete obj->impl;
                    delete obj;
                } break;
                break;
            default:
                std::cerr << "ERROR, mitie_free() called on non-MITIE object or called twice." << std::endl;
                assert(false);
                abort();
        }

    }

// ----------------------------------------------------------------------------------------

    mitie_named_entity_extractor* mitie_load_named_entity_extractor (
        const char* filename
    )
    {
        assert(filename != NULL);

        mitie_named_entity_extractor* obj = 0;
        named_entity_extractor* impl = 0;
        try
        {
            ifstream fin(filename, ios::binary);
            if (!fin)
            {
#ifndef NDEBUG
                cerr << "Error loading MITIE model file, file not found." << endl;
#endif
                return NULL;
            }

            obj = new mitie_named_entity_extractor;
            obj->type = MITIE_NAMED_ENTITY_EXTRACTOR;
            obj->impl = impl = new named_entity_extractor;
            deserialize(*impl, fin);
            return obj;
        }
        catch(std::exception& e)
        {
#ifndef NDEBUG
            cerr << "Error loading MITIE model file: " << e.what() << endl;
#endif
            delete obj;
            delete impl;
            return NULL;
        }
        catch(...)
        {
            delete obj;
            delete impl;
            return NULL;
        }
    }

    unsigned long mitie_get_num_possible_ner_tags (
        const mitie_named_entity_extractor* ner
    )
    {
        assert(ner != NULL);
        return ner->impl->get_tag_name_strings().size();
    }

    const char* mitie_get_named_entity_tagstr (
        const mitie_named_entity_extractor* ner,
        unsigned long idx
    )
    {
        assert(ner != NULL);
        assert(idx < mitie_get_num_possible_ner_tags(ner));
        return ner->impl->get_tag_name_strings()[idx].c_str();
    }

// ----------------------------------------------------------------------------------------

    mitie_named_entity_detections* mitie_extract_entities (
        const mitie_named_entity_extractor* ner,
        const char* text
    )
    {
        assert(ner != NULL);
        assert(text != NULL);

        mitie_named_entity_detections* obj = 0;
        mitie_named_entity_detections_impl* impl = 0;

        try
        {
            obj = new mitie_named_entity_detections;
            obj->type = MITIE_NAMED_ENTITY_DETECTIONS;
            obj->impl = impl = new mitie_named_entity_detections_impl;

            istringstream sin(text);
            unigram_tokenizer tok(sin);
            std::vector<std::string> words;
            std::vector<unsigned long> word_pos;
            string word;
            unsigned long pos;
            while(tok(word, pos))
            {
                words.push_back(word);
                word_pos.push_back(pos);
            }

            (*ner->impl)(words, impl->ranges, impl->predicted_labels);
            impl->tags = ner->impl->get_tag_name_strings();

            // map the ranges from token indices to character indices 
            for (unsigned long i = 0; i < impl->ranges.size(); ++i)
            {
                unsigned long begin, end;
                begin = impl->ranges[i].first;
                end = impl->ranges[i].second;

                begin = word_pos[begin];
                end = word_pos[end-1] + words[end-1].size();

                impl->ranges[i].first = begin;
                impl->ranges[i].second = end;
            }

            return obj;
        }
        catch(...)
        {
            delete obj;
            delete impl;
            return NULL;
        }

    }

    unsigned long mitie_ner_get_num_detections (
        const mitie_named_entity_detections* dets
    )
    {
        assert(dets);
        return dets->impl->ranges.size();
    }

    unsigned long mitie_ner_get_detection_position (
        const mitie_named_entity_detections* dets,
        unsigned long idx
    )
    {
        assert(dets);
        assert(idx < mitie_ner_get_num_detections(dets));
        return dets->impl->ranges[idx].first;
    }

    unsigned long mitie_ner_get_detection_length (
        const mitie_named_entity_detections* dets,
        unsigned long idx
    )
    {
        assert(dets);
        assert(idx < mitie_ner_get_num_detections(dets));
        return dets->impl->ranges[idx].second - dets->impl->ranges[idx].first;
    }

    unsigned long mitie_ner_get_detection_tag (
        const mitie_named_entity_detections* dets,
        unsigned long idx
    )
    {
        assert(dets);
        assert(idx < mitie_ner_get_num_detections(dets));
        return dets->impl->predicted_labels[idx];
    }

    const char* mitie_ner_get_detection_tagstr (
        const mitie_named_entity_detections* dets,
        unsigned long idx
    )
    {
        assert(dets);
        assert(idx < mitie_ner_get_num_detections(dets));
        unsigned long tag = dets->impl->predicted_labels[idx];
        return dets->impl->tags[tag].c_str();
    }

// ----------------------------------------------------------------------------------------

}
