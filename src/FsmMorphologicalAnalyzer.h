//
// Created by Olcay Taner YILDIZ on 25.10.2023.
//

#ifndef MORPHOLOGICALANALYSIS_FSMMORPHOLOGICALANALYZER_H
#define MORPHOLOGICALANALYSIS_FSMMORPHOLOGICALANALYZER_H

#include <Dictionary/Trie/Trie.h>
#include <Dictionary/TxtDictionary.h>
#include "FiniteStateMachine.h"
#include "MorphologicalParse.h"
#include "MetamorphicParse.h"
#include "FsmParse.h"
#include "FsmParseList.h"
#include <Cache/LRUCache.h>
#include <Queue.h>
#include <Sentence.h>

static int MAX_DISTANCE = 2;

struct fsm_morphological_analyzer {
    Trie_ptr dictionary_trie;
    Trie_ptr suffix_trie;
    Finite_state_machine_ptr finite_state_machine;
    Txt_dictionary_ptr dictionary;
    Lru_cache_ptr cache;
    Hash_map_ptr most_used_patterns;
    Hash_map_ptr parsed_surface_forms;
    Hash_map_ptr pronunciations;
};

typedef struct fsm_morphological_analyzer Fsm_morphological_analyzer;

typedef Fsm_morphological_analyzer *Fsm_morphological_analyzer_ptr;

Fsm_morphological_analyzer_ptr
create_fsm_morphological_analyzer(char *fileName, Txt_dictionary_ptr dictionary, int cacheSize);

Fsm_morphological_analyzer_ptr create_fsm_morphological_analyzer2(char *dictionaryFileName, char *fileName);

Fsm_morphological_analyzer_ptr create_fsm_morphological_analyzer3();

void prepare_suffix_tree(Fsm_morphological_analyzer_ptr fsm_morphological_analyzer);

void add_surface_forms(Fsm_morphological_analyzer_ptr fsm_morphological_analyzer, const char *file_name);

void free_fsm_morphological_analyzer(Fsm_morphological_analyzer_ptr fsm_morphological_analyzer);

Hash_set_ptr get_possible_words(Fsm_morphological_analyzer_ptr fsm_morphological_analyzer,
                                const Morphological_parse *morphological_parse,
                                const Metamorphic_parse *metamorphic_parse);

bool is_possible_substring(const char *shortString,
                           const char *longString,
                           Txt_word_ptr root);

void initialize_parse_list(Fsm_morphological_analyzer_ptr fsm_morphological_analyzer,
                           Array_list_ptr fsm_parse,
                           Txt_word_ptr root,
                           bool is_proper);

void initialize_parse_list_from_root(Fsm_morphological_analyzer_ptr fsm_morphological_analyzer,
                                     Array_list_ptr parse_list,
                                     Txt_word_ptr root,
                                     bool is_proper);

Array_list_ptr initialize_parse_list_from_surface_form(Fsm_morphological_analyzer_ptr fsm_morphological_analyzer,
                                                       const char *surface_form,
                                                       bool is_proper);

void add_new_parses_from_current_parse(Fsm_morphological_analyzer_ptr fsm_morphological_analyzer,
                                       Fsm_parse_ptr current_fsm_parse,
                                       Queue_ptr fsm_parse_queue,
                                       int max_length,
                                       Txt_word_ptr root);

void add_new_parses_from_current_parse2(Fsm_morphological_analyzer_ptr fsm_morphological_analyzer,
                                        Fsm_parse_ptr current_fsm_parse,
                                        Queue_ptr fsm_parse_queue,
                                        char *surface_form,
                                        Txt_word_ptr root);

bool
parse_exists(Fsm_morphological_analyzer_ptr fsm_morphological_analyzer, Array_list_ptr fsm_parses, char *surface_form);

Array_list_ptr
parse_word(Fsm_morphological_analyzer_ptr fsm_morphological_analyzer, Array_list_ptr fsm_parses, int max_length);

Array_list_ptr
parse_word2(Fsm_morphological_analyzer_ptr fsm_morphological_analyzer, Array_list_ptr fsm_parses, char *surface_form);

Array_list_ptr morphological_analysis2(Fsm_morphological_analyzer_ptr fsm_morphological_analyzer,
                                       Txt_word_ptr root,
                                       char *surface_form,
                                       char *state);

Array_list_ptr
generate_all_parses(Fsm_morphological_analyzer_ptr fsm_morphological_analyzer, Txt_word_ptr root, int max_length);

Array_list_ptr morphological_analysis3(Fsm_morphological_analyzer_ptr fsm_morphological_analyzer, Txt_word_ptr root,
                                       char *surface_form);

bool pattern_matches(Fsm_morphological_analyzer_ptr fsm_morphological_analyzer, char *expr, char *value);

bool is_proper_noun_fsm(const char *surface_form);

bool is_code_fsm(Fsm_morphological_analyzer_ptr fsm_morphological_analyzer, char *surface_form);

bool is_integer(Fsm_morphological_analyzer_ptr fsm_morphological_analyzer, char *surface_form);

bool is_double(Fsm_morphological_analyzer_ptr fsm_morphological_analyzer, char *surface_form);

bool is_number_fsm(Fsm_morphological_analyzer_ptr fsm_morphological_analyzer, char *surface_form);

bool is_percent_fsm(Fsm_morphological_analyzer_ptr fsm_morphological_analyzer, char *surface_form);

bool is_time_fsm(Fsm_morphological_analyzer_ptr fsm_morphological_analyzer, char *surface_form);

bool is_range_fsm(Fsm_morphological_analyzer_ptr fsm_morphological_analyzer, char *surface_form);

bool is_date_fsm(Fsm_morphological_analyzer_ptr fsm_morphological_analyzer, char *surface_form);

Sentence_ptr replace_word_fsm(Fsm_morphological_analyzer_ptr fsm_morphological_analyzer,
                              Sentence_ptr original,
                              char *previous_word,
                              char *new_word);

bool analysis_exists(Fsm_morphological_analyzer_ptr fsm_morphological_analyzer,
                     Txt_word_ptr root_word,
                     char *surface_form,
                     bool is_proper);

Array_list_ptr analysis(Fsm_morphological_analyzer_ptr fsm_morphological_analyzer, char *surface_form, bool is_proper);

Array_list_ptr root_of_possibly_new_word(Fsm_morphological_analyzer_ptr fsm_morphological_analyzer, char *surface_form);

Fsm_parse_list_ptr
robust_morphological_analysis(Fsm_morphological_analyzer_ptr fsm_morphological_analyzer, char *surfaceForm);

Fsm_parse_list_ptr *
morphological_analysis4(Fsm_morphological_analyzer_ptr fsm_morphological_analyzer, Sentence_ptr sentence);

Fsm_parse_list_ptr *
robust_morphological_analysis2(Fsm_morphological_analyzer_ptr fsm_morphological_analyzer, Sentence_ptr sentence);

bool morphological_analysis_exists(Fsm_morphological_analyzer_ptr fsm_morphological_analyzer, Txt_word_ptr rootWord,
                                   char *surfaceForm);

Fsm_parse_list_ptr morphological_analysis(Fsm_morphological_analyzer_ptr fsm_morphological_analyzer, char *surfaceForm);

void add_pronunciations(Fsm_morphological_analyzer_ptr fsm_morphological_analyzer, char* file_name);

#endif //MORPHOLOGICALANALYSIS_FSMMORPHOLOGICALANALYZER_H
