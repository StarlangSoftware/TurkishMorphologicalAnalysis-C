//
// Created by Olcay Taner YILDIZ on 23.10.2023.
//

#ifndef MORPHOLOGICALANALYSIS_FSMPARSE_H
#define MORPHOLOGICALANALYSIS_FSMPARSE_H

#include <Dictionary/Word.h>
#include <Dictionary/TxtWord.h>
#include "FsmState.h"
#include "InflectionalGroup.h"

struct fsm_parse{
    Array_list_ptr inflectional_groups;
    Txt_word_ptr root;
    Array_list_ptr suffix_list;
    Array_list_ptr form_list;
    Array_list_ptr transition_list;
    Array_list_ptr with_list;
    char* initial_pos;
    char* pos;
    char* form;
    char* verb_agreement;
    char* possessive_agreement;
};

typedef struct fsm_parse Fsm_parse;

typedef Fsm_parse *Fsm_parse_ptr;

Fsm_parse_ptr create_fsm_parse(Txt_word_ptr root);

Fsm_parse_ptr create_fsm_parse2();

void free_fsm_parse(Fsm_parse_ptr fsm_parse);

Fsm_parse_ptr create_fsm_parse3(int number, Fsm_State_ptr start_state);

Fsm_parse_ptr create_fsm_parse4(double  number, Fsm_State_ptr start_state);

void update_fsm_parse_with_state_and_name(Fsm_parse_ptr fsm_parse, char* name, Fsm_State_ptr start_state);

Fsm_parse_ptr create_fsm_parse5(char* punctuation, Fsm_State_ptr start_state);

Fsm_parse_ptr create_fsm_parse6(Txt_word_ptr root, Fsm_State_ptr start_state);

void construct_inflectional_groups(Fsm_parse_ptr fsm_parse);

void set_agreement(Fsm_parse_ptr fsm_parse, char* transition_name);

char* get_last_lemma_with_tag(Fsm_parse_ptr fsm_parse, char* _pos);

char* get_last_lemma(Fsm_parse_ptr fsm_parse);

void add_suffix(Fsm_parse_ptr fsm_parse,
                Fsm_State_ptr suffix,
                char* _form,
                char* transition,
                char* with,
                char* toPos);

Fsm_State_ptr get_start_state(Fsm_parse_ptr fsm_parse);

void set_form(Fsm_parse_ptr fsm_parse, char* name);

Fsm_State_ptr get_final_suffix(Fsm_parse_ptr fsm_parse);

String_ptr header_transition(const Fsm_parse* fsm_parse);

String_ptr pronoun_transition(const Fsm_parse* fsm_parse);

char* transition_list(const Fsm_parse* fsm_parse);

char* get_suffix_list(Fsm_parse_ptr fsm_parse);

char* get_with_list(Fsm_parse_ptr fsm_parse);

char* replace_root_word(Fsm_parse_ptr fsm_parse, Txt_word_ptr new_root);

char* fsm_parse_to_string(Fsm_parse_ptr fsm_parse);

int compare_fsm_parse(const Fsm_parse* fsm_parse1, const Fsm_parse* fsm_parse2);

char* get_word_with_pos2(const Fsm_parse * fsm_parse);

Inflectional_group_ptr first_inflectional_group2(const Fsm_parse * fsm_parse);

Fsm_parse_ptr clone_fsm_parse(const Fsm_parse * fsm_parse);

bool fsm_parse_contains_tag(const Fsm_parse* fsm_parse, Morphological_tag tag);

bool is_fsm_parse_noun(const Fsm_parse* fsm_parse);

bool is_fsm_parse_plural(const Fsm_parse *fsm_parse);

char* get_fsm_parse_pos(const Fsm_parse *fsm_parse);

bool is_fsm_parse_capital_word(const Fsm_parse *fsm_parse);

void restore_original_form(Fsm_parse *fsm_parse, char* original, char* pronunciation);

#endif //MORPHOLOGICALANALYSIS_FSMPARSE_H
