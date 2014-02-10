// Created by Davis E. King on Feb 10, 2014
#ifndef MIT_LL_MITIE_NaMED_ENTITY_EXTRACTOR_H__
#define MIT_LL_MITIE_NaMED_ENTITY_EXTRACTOR_H__

#include <mitie/total_word_feature_extractor.h>
#include <mitie/ner_feature_extraction.h>
#include <dlib/svm.h>

namespace mitie
{
    class named_entity_extractor
    {
        /*!
            WHAT THIS OBJECT REPRESENTS
                This object is a simple tool for identifying the named entities in
                tokenized text.  In particular, it's just a wrapper around a
                dlib::sequence_segmenter and a multiclass classifier that predicts the type
                of each named entity.

            THREAD SAFETY
                Note that this object uses mutable internal scratch space.  Therefore, it is
                unsafe for two threads to touch the same instance of this object at a time
                without mutex locking it first.
        !*/
    public:

        named_entity_extractor(){}
        /*!
            ensures
                - When used this object won't output any entities.   You need to either use
                  the other constructor or deserialize a saved named_entity_extractor to
                  get something that is useful.
        !*/

        named_entity_extractor(
            const std::vector<std::string>& tag_name_strings,
            const total_word_feature_extractor& fe,
            const dlib::sequence_segmenter<ner_feature_extractor>& segmenter,
            const dlib::multiclass_linear_decision_function<dlib::sparse_linear_kernel<ner_sample_type>,unsigned long>& df
        ); 
        /*!
            requires
                - segmenter.get_feature_extractor().num_features() == fe.get_num_dimensions() 
                - df must be designed to work with fe (i.e. it must have been trained with
                  features from fe and extract_ner_chunk_features()).
                - df.number_of_classes() == tag_name_strings.size()+1
                  (i.e. the classifier needs to predict all the possible tags and also a final "not entity" tag)
                - for all i where 0 <= i <= tag_name_strings.size():
                    - df.get_labels() contains an element equal to i.
                      (All we are saying there is that the labels need to be contiguous
                      integers and that the tag_name_strings vector and the decision function
                      need to agree on the set of labels)
            ensures
                - Just loads the given objects into *this.  
                - The interpretation of tag_name_strings is that it maps the output of df
                  into a meaningful text name for the NER tag.  
        !*/

        void operator() (
            const std::vector<std::string>& sentence,
            std::vector<std::pair<unsigned long, unsigned long> >& chunks,
            std::vector<unsigned long>& chunk_tags
        ) const;
        /*!
            ensures
                - Runs the named entity recognizer on the sequence of tokenized words
                  inside sentence.  The detected named entities are stored into chunks.  
                - #chunks == the locations of the named entities. 
                - #chunks.size() == #chunk_tags.size()
                - for all valid i:
                    - #chunk_tags[i] == the label for the entity at location #chunks[i].  Moreover, 
                      chunk tag ID numbers are contiguous and start at 0.  Therefore we have:
                        - 0 <= #chunk_tags[i] < get_tag_name_strings().size()
                    - #chunks[i] == a half open range indicating where the entity is within
                      sentence.  In particular, the entity is composed of the tokens
                      sentence[#chunks[i].first] through sentence[#chunks[i].second-1].
                    - The textual label for the i-th entity is get_tag_name_strings()[#chunk_tags[i]].
        !*/

        const std::vector<std::string>& get_tag_name_strings (
        ) const { return tag_name_strings; }
        /*!
            ensures
                - Returns a vector that maps entity numeric ID tags into their string labels.  
        !*/

        friend void serialize(const named_entity_extractor& item, std::ostream& out)
        {
            int version = 1;
            dlib::serialize(version, out);
            dlib::serialize(item.tag_name_strings, out);
            serialize(item.fe, out);
            serialize(item.segmenter, out);
            serialize(item.df, out);
        }

        friend void deserialize(named_entity_extractor& item, std::istream& in)
        {
            int version = 0;
            dlib::deserialize(version, in);
            if (version != 1)
                throw dlib::serialization_error("Unexpected version found while deserializing mitie::named_entity_extractor.");
            dlib::deserialize(item.tag_name_strings, in);
            deserialize(item.fe, in);
            deserialize(item.segmenter, in);
            deserialize(item.df, in);
        }

    private:
        std::vector<std::string> tag_name_strings;
        total_word_feature_extractor fe;
        dlib::sequence_segmenter<ner_feature_extractor> segmenter;
        dlib::multiclass_linear_decision_function<dlib::sparse_linear_kernel<ner_sample_type>,unsigned long> df;
    };
}

#endif // MIT_LL_MITIE_NaMED_ENTITY_EXTRACTOR_H__

