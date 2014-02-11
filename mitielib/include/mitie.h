// Created by Davis E. King on Feb 11, 2014
#ifndef MITLL_MITIe_H__
#define MITLL_MITIe_H__

extern "C"
{
    /*!
        MITIE RESOURCE MANAGEMENT POLICY
            Unless explicitly noted, you do NOT need to call free() or mitie_free() on the
            pointers returned from any MITIE API calls.  That is, if it is the caller's
            responsibility to free an object created by a MITIE API call then the
            documentation for that routine will explicitly say the caller needs to free the
            object.
    !*/

// ----------------------------------------------------------------------------------------

    typedef struct mitie_named_entity_extractor  mitie_named_entity_extractor;
    typedef struct mitie_named_entity_detections mitie_named_entity_detections;

    void mitie_free (
        void* object 
    );
    /*!
        ensures
            - Frees the resources associated with any MITIE object.
    !*/

// ----------------------------------------------------------------------------------------

    mitie_named_entity_extractor* mitie_load_named_entity_extractor (
        const char* filename
    );
    /*!
        requires
            - filename == a valid pointer to a null terminated C string
        ensures
            - The returned object MUST BE FREED by a call to mitie_free().
            - If the object can't be created then this function returns NULL
    !*/

    unsigned long mitie_get_num_possible_ner_tags (
        const mitie_named_entity_extractor* ner
    );
    /*!
        requires
            - ner != NULL
        ensures
            - A named entity extractor tags each entity with a tag.  This function returns
              the number of different tags which can be produced by the given named entity
              extractor.  Moreover, each tag is uniquely identified by a numeric ID which
              is just the index of the tag.  For example, if there are 4 possible tags then
              the numeric IDs are just 0, 1, 2, and 3.  
    !*/

    const char* mitie_get_named_entity_tagstr (
        const mitie_named_entity_extractor* ner,
        unsigned long idx
    );
    /*!
        requires
            - ner != NULL
            - idx < mitie_get_num_possible_ner_tags(ner)
        ensures
            - Each named entity tag, in addition to having a numeric ID which uniquely
              identifies it, has a text string name.  For example, if a named entity tag
              logically identifies a person then the tag string might be "PERSON". 
            - This function takes a tag ID number and returns the tag string for that tag.
            - The returned pointer is valid until mitie_free(ner) is called.
    !*/

// ----------------------------------------------------------------------------------------

    mitie_named_entity_detections* mitie_extract_entities (
        const mitie_named_entity_extractor* ner,
        const char* text
    );
    /*!
        requires
            - ner != NULL
            - text == a null terminated C string
        ensures
            - The returned object MUST BE FREED by a call to mitie_free().
            - Runs the supplied named entity extractor on text and returns a set of
              named entity detections.
            - If the object can't be created then this function returns NULL
    !*/

    unsigned long mitie_ner_get_num_detections (
        const mitie_named_entity_detections* dets
    );
    /*!
        requires
            - dets != NULL
        ensures
            - returns the number of named entity detections inside the dets object.
    !*/

    unsigned long mitie_ner_get_detection_position (
        const mitie_named_entity_detections* dets,
        unsigned long idx
    );
    /*!
        requires
            - dets != NULL
            - idx < mitie_ner_get_num_detections(dets)
        ensures
            - This function returns the position of the idx-th named entity within the
              input text.  That is, if dets was created by calling
              mitie_extract_entities(ner, TEXT) then the return value of
              mitie_ner_get_detection_position() is an index I such that TEXT[I] is the
              first character in the input text that is part of the named entity.
            - The named entity detections are stored in the order they appeared in the
              input text.  That is, for all valid IDX it is true that:
                - mitie_ner_get_detection_position(dets,IDX) < mitie_ner_get_detection_position(dets,IDX+1)
    !*/

    unsigned long mitie_ner_get_detection_length (
        const mitie_named_entity_detections* dets,
        unsigned long idx
    );
    /*!
        requires
            - dets != NULL
            - idx < mitie_ner_get_num_detections(dets)
        ensures
            - returns the length of the idx-th named entity.  That is, this function
              returns the number of chars from the input text which comprise the idx-th
              named entity detection.  
    !*/

    unsigned long mitie_ner_get_detection_tag (
        const mitie_named_entity_detections* dets,
        unsigned long idx
    );
    /*!
        requires
            - dets != NULL
            - idx < mitie_ner_get_num_detections(dets)
        ensures
            - returns a numeric value that identifies the type of the idx-th named entity.
    !*/

    const char* mitie_ner_get_detection_tagstr (
        const mitie_named_entity_detections* dets,
        unsigned long idx
    );
    /*!
        requires
            - dets != NULL
            - idx < mitie_ner_get_num_detections(dets)
        ensures
            - returns a NULL terminated C string that identifies the type of the idx-th
              named entity. 
            - The returned pointer is valid until mitie_free(dets) is called.
    !*/

// ----------------------------------------------------------------------------------------

}

#endif // MITLL_MITIe_H__

