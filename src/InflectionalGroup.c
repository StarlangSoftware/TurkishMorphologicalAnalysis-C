//
// Created by Olcay Taner YILDIZ on 14.10.2023.
//

#include <stdlib.h>
#include <StringUtils.h>
#include <string.h>
#include <stdio.h>
#include <FileUtils.h>
#include "InflectionalGroup.h"

/**
 * A constructor of InflectionalGroup class which initializes the IG vector by parsing given input
 * String IG by + and calling the getMorphologicalTag method with these substrings. If getMorphologicalTag method returns
 * a tag, it adds this tag to the IG vector.
 *
 * @param IG String input.
 */
Inflectional_group_ptr create_inflectional_group(const char *IG) {
    Inflectional_group_ptr result = malloc(sizeof(Inflectional_group));
    result->IG = create_array_list();
    Array_list_ptr morphological_tags = str_split(IG, '+');
    for (int i = 0; i < morphological_tags->size; i++){
        char* s = array_list_get(morphological_tags, i);
        Morphological_tag* tag = malloc(sizeof(Morphological_tag));
        *tag = get_morphological_tag(s);
        array_list_add(result->IG, tag);
    }
    free_array_list(morphological_tags, free);
    return result;
}

void free_inflectional_group(Inflectional_group_ptr inflectional_group) {
    free_array_list(inflectional_group->IG, NULL);
    free(inflectional_group);
}

/**
 * The getMorphologicalTag method takes a String tag as an input and if the input matches with one of the elements of
 * tags array, it then gets the morphoTags of this tag and returns it.
 *
 * @param tag String to get morphoTags from.
 * @return morphoTags if found, null otherwise.
 */
Morphological_tag get_morphological_tag(const char *tag) {
   Morphological_tag  result = NOTAG;
   char* _tag = uppercase_en(tag);
   for (int i = 0; i < 131; i++){
       char* uppercase = uppercase_en(tags[i]);
       if (strcmp(_tag, uppercase) == 0){
           free(uppercase);
           result = morpho_tags[i];
           break;
       }
       free(uppercase);
   }
   free(_tag);
   return result;
}

/**
 * The getTag method takes a MorphologicalTag type tag as an input and returns its corresponding tag from tags array.
 *
 * @param tag MorphologicalTag type input to find tag from.
 * @return tag if found, null otherwise.
 */
char *get_tag(Morphological_tag tag) {
    for (int i = 0; i < 131; i++){
        if (tag == morpho_tags[i]){
            return uppercase_en(tags[i]);
        }
    }
    return NULL;
}

/**
 * Another getTag method which takes index as an input and returns the corresponding tag from IG vector.
 *
 * @param index to get tag.
 * @return tag at input index.
 */
Morphological_tag get_tag_with_index(const Inflectional_group* inflectional_group, int index) {
    Morphological_tag* tag = array_list_get(inflectional_group->IG, index);
    return *tag;
}

/**
 * The containsCase method loops through the tags in IG ArrayList and finds out the tags of the NOMINATIVE,
 * ACCUSATIVE, DATIVE, LOCATIVE or ABLATIVE cases.
 *
 * @return tag which holds the condition.
 */
Morphological_tag contains_case(const Inflectional_group *inflectional_group) {
    for (int i = 0; i < inflectional_group->IG->size; i++){
        Morphological_tag* tag = array_list_get(inflectional_group->IG, i);
        if (*tag == NOMINATIVE || *tag == ACCUSATIVE || *tag == DATIVE || *tag == LOCATIVE || *tag == ABLATIVE){
            return *tag;
        }
    }
    return NOTAG;
}

/**
 * The containsPlural method loops through the tags in IG vector and checks whether the tags are from
 * the agreement plural or possessive plural, i.e A1PL, A2PL, A3PL, P1PL, P2PL and P3PL.
 *
 * @return true if the tag is plural, false otherwise.
 */
bool contains_plural(const Inflectional_group *inflectional_group) {
    for (int i = 0; i < inflectional_group->IG->size; i++){
        Morphological_tag* tag = array_list_get(inflectional_group->IG, i);
        if (*tag == A1PL || *tag == A2PL || *tag == A3PL || *tag == P1PL || *tag == P2PL || *tag == P3PL){
            return true;
        }
    }
    return false;
}

/**
 * The containsTag method takes a MorphologicalTag type tag as an input and loops through the tags in
 * IG vector and returns true if the input matches with on of the tags in the IG.
 *
 * @param _tag MorphologicalTag type input to search for.
 * @return true if tag matches with the tag in IG, false otherwise.
 */
bool contains_tag(const Inflectional_group *inflectional_group, Morphological_tag _tag) {
    for (int i = 0; i < inflectional_group->IG->size; i++){
        Morphological_tag* tag = array_list_get(inflectional_group->IG, i);
        if (*tag == _tag){
            return true;
        }
    }
    return false;
}

/**
 * The containsPossessive method loops through the tags in IG ArrayList and returns true if the tag in IG is
 * one of the possessives: P1PL, P1SG, P2PL, P2SG, P3PL AND P3SG.
 *
 * @return true if it contains possessive tag, false otherwise.
 */
bool contains_possessive(const Inflectional_group *inflectional_group) {
    for (int i = 0; i < inflectional_group->IG->size; i++){
        Morphological_tag* tag = array_list_get(inflectional_group->IG, i);
        if (*tag == P1PL || *tag == P1SG || *tag == P2PL || *tag == P2SG || *tag == P3PL || *tag == P3SG){
            return true;
        }
    }
    return false;
}

/**
 * Overridden toString method to return resulting tags in IG vector.
 *
 * @return String result.
 */
char *inflectional_group_to_string(const Inflectional_group *inflectional_group) {
    char tmp[MAX_LINE_LENGTH];
    sprintf(tmp, "%s", get_tag(*(Morphological_tag*) array_list_get(inflectional_group->IG, 0)));
    for (int i = 1; i < inflectional_group->IG->size; i++){
        sprintf(tmp, "%s+%s", tmp, get_tag(*(Morphological_tag*) array_list_get(inflectional_group->IG, i)));
    }
    char *result = NULL;
    result = str_copy(result, tmp);
    return result;
}
