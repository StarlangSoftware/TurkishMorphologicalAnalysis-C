//
// Created by Olcay Taner YILDIZ on 18.10.2023.
//

#ifndef MORPHOLOGICALANALYSIS_DISAMBIGUATEDWORD_H
#define MORPHOLOGICALANALYSIS_DISAMBIGUATEDWORD_H

#include "MorphologicalParse.h"

struct disambiguated_word{
    char* name;
    Morphological_parse_ptr parse;
};

typedef struct disambiguated_word Disambiguated_word;

typedef Disambiguated_word *Disambiguated_word_ptr;

Disambiguated_word_ptr create_disambiguated_word(const char* name, Morphological_parse_ptr parse);

void free_disambiguated_word(Disambiguated_word_ptr disambiguated_word);

#endif //MORPHOLOGICALANALYSIS_DISAMBIGUATEDWORD_H
