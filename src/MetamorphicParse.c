//
// Created by Olcay Taner YILDIZ on 15.10.2023.
//

#include <string.h>
#include <stdlib.h>
#include <FileUtils.h>
#include "MetamorphicParse.h"

/**
 * A constructor of MetamorphicParse class which creates an vector metaMorphemeList which has split words
 * according to +.
 *
 * @param parse String to parse.
 */
Metamorphic_parse_ptr create_metamorphic_parse(const char *parse) {
    Metamorphic_parse_ptr result = malloc(sizeof(Metamorphic_parse));
    result->meta_morpheme_list = create_array_list();
    if (strcmp(parse, "+") == 0) {
        result->root = create_word("+");
    } else {
        Array_list_ptr words = str_split(parse, '+');
        result->root = create_word(array_list_get(words, 0));
        for (int i = 1; i < words->size; i++){
            array_list_add(result->meta_morpheme_list, array_list_get(words, i));
        }
        free_array_list(words, NULL);
    }
    return result;
}

/**
 * The getMetaMorphemeTag method takes a String tag as an input and takes the first char of the tag. If first char
 * is a punctuation it gets a substring from the tag. And gets the meta morphemes of this tag then adds to the
 * result vector.
 *
 * @param tag String to get meta morphemes from.
 * @return vector type result which holds meta morphemes.
 */
Array_list_ptr get_meta_morpheme_tag1(char *tag) {
    Array_list_ptr result = create_array_list();
    String_ptr st = char_at(tag, 0);
    char* _tag = tag;
    if (is_punctuation(st->s)) {
        _tag = substring(tag, 1, word_size(tag))->s;
    }
    free_string_ptr(st);
    for (int j = 0; j < 109; j++) {
        if (strcmp(_tag, meta_morphemes[j]) == 0) {
            Morphological_tag* tag_ptr = malloc(sizeof(Morphological_tag));
            *tag_ptr = morphotactic_tags[j];
            array_list_add(result, tag_ptr);
        }
    }
    return result;
}

void free_metamorphic_parse(Metamorphic_parse_ptr metamorphic_parse) {
    free_array_list(metamorphic_parse->meta_morpheme_list, free);
    free_word(metamorphic_parse->root);
    free(metamorphic_parse);
}

/**
 * The addMetaMorphemeList method splits input String by + and add to the metaMorphemeList.
 *
 * @param newTacticSet String to add the metaMorphemeList.
 */
void add_meta_morpheme_list(Metamorphic_parse_ptr metamorphic_parse, const char *new_tactic_set) {
    Array_list_ptr tactics = str_split(new_tactic_set, '+');
    array_list_add_all(metamorphic_parse->meta_morpheme_list, tactics);
}

/**
 * The removeMetaMorphemeFromIndex method removes the meta morpheme at given index from metaMorphemeList.
 *
 * @param index to remove from metaMorphemeList.
 */
void remove_meta_morpheme_from_index(Metamorphic_parse_ptr metamorphic_parse, int index) {
    while (index - 1 < metamorphic_parse->meta_morpheme_list->size){
        array_list_remove(metamorphic_parse->meta_morpheme_list, index - 1, free);
    }
}

/**
 * The getMetaMorpheme method gets the meta morpheme at given index.
 *
 * @param index is used to get the meta morpheme.
 * @return metaMorphemeList's corresponding meta morpheme.
 */
char *get_meta_morpheme(const Metamorphic_parse* metamorphic_parse, int index) {
    if (index == 0) {
        return metamorphic_parse->root->name;
    } else {
        return array_list_get(metamorphic_parse->meta_morpheme_list, index - 1);
    }
}

/**
 * Overridden toString method to return resulting meta morphemes in metaMorphemeList.
 *
 * @return String result.
 */
char *metamorphic_parse_to_string(const Metamorphic_parse* metamorphic_parse) {
    char tmp[MAX_LINE_LENGTH];
    sprintf(tmp, "%s", metamorphic_parse->root->name);
    for (int i = 0; i < metamorphic_parse->meta_morpheme_list->size; i++){
        sprintf(tmp, "%s+%s", tmp, (char*) array_list_get(metamorphic_parse->meta_morpheme_list, i));
    }
    char *result = NULL;
    result = str_copy(result, tmp);
    return result;
}
