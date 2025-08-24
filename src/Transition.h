//
// Created by Olcay Taner YILDIZ on 19.10.2023.
//

#ifndef MORPHOLOGICALANALYSIS_TRANSITION_H
#define MORPHOLOGICALANALYSIS_TRANSITION_H

#include <Dictionary/TxtWord.h>
#include "FsmState.h"
#include "FsmParse.h"

struct transition{
    Fsm_State_ptr to_state;
    char* with;
    char* with_name;
    char* to_pos;
};

typedef struct transition Transition;

typedef Transition *Transition_ptr;

Transition_ptr create_transition(Fsm_State_ptr to_state, const char* with, const char* with_name);

Transition_ptr create_transition2(Fsm_State_ptr to_state, const char* with, const char* with_name, const char* to_pos);

Transition_ptr create_transition3(const char* with);

void free_transition(Transition_ptr transition);

bool transition_possible1(Transition_ptr transition, const char* current_surface_form, const char* real_surface_form);

bool transition_possible2(Transition_ptr transition, Fsm_parse_ptr current_fsm_parse);

bool transition_possible3(Transition_ptr transition, Txt_word_ptr root, Fsm_State_ptr from_state);

char* with_first_char(Transition_ptr transition);

bool start_with_vowel_or_consonant_drops(Transition_ptr transition);

bool soften_during_suffixation(Transition_ptr transition, Txt_word_ptr root, Fsm_State_ptr start_state);

char* make_transition(Transition_ptr transition, Txt_word_ptr root, const char* stem);

char* make_transition2(Transition_ptr transition, Txt_word_ptr root, const char* stem, Fsm_State_ptr startState);

#endif //MORPHOLOGICALANALYSIS_TRANSITION_H
