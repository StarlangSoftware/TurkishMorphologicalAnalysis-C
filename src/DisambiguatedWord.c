//
// Created by Olcay Taner YILDIZ on 18.10.2023.
//

#include <stdlib.h>
#include "DisambiguatedWord.h"

/**
 * The constructor of DisambiguatedWord class which takes a String and a MorphologicalParse
 * as inputs. It creates a new MorphologicalParse with given MorphologicalParse. It generates a new instance with
 * given String.
 *
 * @param name  Instances that will be a DisambiguatedWord.
 * @param parse MorphologicalParse of the DisambiguatedWord.
 */
Disambiguated_word_ptr create_disambiguated_word(const char *name, Morphological_parse_ptr parse) {
    Disambiguated_word_ptr result = malloc(sizeof(Disambiguated_word));
    result->name = str_copy(result->name, name);
    result->parse = parse;
    return result;
}

void free_disambiguated_word(Disambiguated_word_ptr disambiguated_word) {
    free(disambiguated_word->name);
    free_morphological_parse(disambiguated_word->parse);
    free(disambiguated_word);
}
