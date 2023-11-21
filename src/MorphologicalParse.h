//
// Created by Olcay Taner YILDIZ on 16.10.2023.
//

#ifndef MORPHOLOGICALANALYSIS_MORPHOLOGICALPARSE_H
#define MORPHOLOGICALANALYSIS_MORPHOLOGICALPARSE_H

#include <ArrayList.h>
#include <Dictionary/Word.h>
#include "InflectionalGroup.h"

struct morphological_parse{
    Array_list_ptr inflectional_groups;
    char* root;
};

typedef struct morphological_parse Morphological_parse;

typedef Morphological_parse *Morphological_parse_ptr;

Morphological_parse_ptr create_morphological_parse(const char* parse);

Morphological_parse_ptr create_morphological_parse2(const Array_list* inflectional_groups);

void free_morphological_parse(Morphological_parse_ptr morphological_parse);

void update_root_and_inflectional_groups(Morphological_parse_ptr morphological_parse, const Array_list* inflectional_groups);

char* get_transition_list(const Morphological_parse* morphological_parse);

char* get_inflectional_group_string(const Morphological_parse* morphological_parse, int index);

Inflectional_group_ptr get_inflectional_group(const Morphological_parse* morphological_parse, int index);

Inflectional_group_ptr get_last_inflectional_group(const Morphological_parse* morphological_parse);

char* get_tag_for_index(const Morphological_parse* morphological_parse, int index);

int tag_size(const Morphological_parse* morphological_parse);

Inflectional_group_ptr first_inflectional_group(const Morphological_parse* morphological_parse);

Inflectional_group_ptr last_inflectional_group(const Morphological_parse* morphological_parse);

char* get_word_with_pos(const Morphological_parse* morphological_parse);

char* get_pos(const Morphological_parse* morphological_parse);

char* get_root_pos(const Morphological_parse* morphological_parse);

char* last_ig_contains_case(const Morphological_parse* morphological_parse);

bool last_ig_contains_tag(const Morphological_parse* morphological_parse, Morphological_tag tag);

bool last_ig_contains_possessive(const Morphological_parse* morphological_parse);

bool is_capital_word(const Morphological_parse* morphological_parse);

bool is_noun(const Morphological_parse* morphological_parse);

bool is_parse_verb(const Morphological_parse* morphological_parse);

bool is_root_verb(const Morphological_parse* morphological_parse);

bool is_parse_adjective(const Morphological_parse* morphological_parse);

bool is_parse_proper_noun(const Morphological_parse* morphological_parse);

bool is_parse_punctuation(const Morphological_parse* morphological_parse);

bool is_cardinal(const Morphological_parse* morphological_parse);

bool is_parse_ordinal(const Morphological_parse* morphological_parse);

bool is_parse_real(const Morphological_parse* morphological_parse);

bool is_number(const Morphological_parse* morphological_parse);

bool is_parse_time(const Morphological_parse* morphological_parse);

bool is_parse_date(const Morphological_parse* morphological_parse);

bool is_hash_tag(const Morphological_parse* morphological_parse);

bool is_email(const Morphological_parse* morphological_parse);

bool is_parse_percent(const Morphological_parse* morphological_parse);

bool is_parse_fraction(const Morphological_parse* morphological_parse);

bool is_parse_range(const Morphological_parse* morphological_parse);

bool is_parse_plural(const Morphological_parse* morphological_parse);

bool is_auxiliary(const Morphological_parse* morphological_parse);

bool parse_contains_tag(const Morphological_parse* morphological_parse, Morphological_tag tag);

char* get_tree_pos(const Morphological_parse* morphological_parse);

char* get_pron_type(const Morphological_parse* morphological_parse);

char* get_num_type(const Morphological_parse* morphological_parse);

char* get_reflex(const Morphological_parse* morphological_parse);

char* get_number(const Morphological_parse* morphological_parse);

char* get_case(const Morphological_parse* morphological_parse);

char* get_definite(const Morphological_parse* morphological_parse);

char* get_degree(const Morphological_parse* morphological_parse);

char* get_polarity(const Morphological_parse* morphological_parse);

char* get_person(const Morphological_parse* morphological_parse);

char* get_voice(const Morphological_parse* morphological_parse);

char* get_aspect(const Morphological_parse* morphological_parse);

char* get_tense(const Morphological_parse* morphological_parse);

char* get_mood(const Morphological_parse* morphological_parse);

char* get_verb_form(const Morphological_parse* morphological_parse);

Array_list_ptr get_universal_dependency_features(const Morphological_parse* morphological_parse, const char* u_pos);

char* get_universal_dependency_pos(const Morphological_parse* morphological_parse);

char* morphological_parse_to_string(const Morphological_parse* morphological_parse);

#endif //MORPHOLOGICALANALYSIS_MORPHOLOGICALPARSE_H
