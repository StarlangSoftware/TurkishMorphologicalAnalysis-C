//
// Created by Olcay Taner YILDIZ on 18.10.2023.
//

#ifndef MORPHOLOGICALANALYSIS_MORPHOTACTICENGINE_H
#define MORPHOLOGICALANALYSIS_MORPHOTACTICENGINE_H

#include "Dictionary/TxtWord.h"

void resolve_D(Txt_word_ptr root, String_ptr formation, const char *formation_to_check);

void resolve_A(Txt_word_ptr root, String_ptr formation, bool root_word, const char *formation_to_check);

void resolve_H(Txt_word_ptr root,
               String_ptr formation,
               bool beginning_of_suffix,
               bool special_case_tense_suffix,
               bool root_word,
               const char *formation_to_check);

void resolve_C(String_ptr formation, const char *formationToCheck);

void resolve_S(String_ptr formation);

void resolve_Sh(String_ptr formation);

#endif //MORPHOLOGICALANALYSIS_MORPHOTACTICENGINE_H
