//
// Created by Olcay Taner YILDIZ on 25.10.2023.
//

#include <stdlib.h>
#include <FileUtils.h>
#include <string.h>
#include <RegularExpression.h>
#include <Memory/Memory.h>
#include "FsmMorphologicalAnalyzer.h"
#include "FsmParseList.h"
#include "Transition.h"

/**
 * Another constructor of FsmMorphologicalAnalyzer class. It generates a new TxtDictionary type dictionary from
 * given input dictionary, with given inputs fileName and cacheSize.
 *
 * @param fileName   the file to read the finite fsm_state machine.
 * @param dictionary the dictionary file that will be used to generate dictionaryTrie.
 * @param cacheSize  the size of the LRUCache.
 */
Fsm_morphological_analyzer_ptr  create_fsm_morphological_analyzer(char *fileName,
                                                                  Txt_dictionary_ptr dictionary,
                                                                  int cacheSize) {
    Fsm_morphological_analyzer_ptr result = malloc_(sizeof(Fsm_morphological_analyzer), "create_fsm_morphological_analyzer");
    result->finite_state_machine = create_finite_state_machine(fileName);
    result->dictionary_trie = prepare_trie(dictionary);
    prepare_suffix_tree(result);
    result->dictionary = dictionary;
    result->parsed_surface_forms = NULL;
    result->cache = create_lru_cache(cacheSize, (unsigned int (*)(const void *, int)) hash_function_string,
                                     (int (*)(const void *, const void *)) compare_string);
    result->most_used_patterns = create_string_hash_map();
    return result;
}

/**
 * Another constructor of FsmMorphologicalAnalyzer class. It generates a new TxtDictionary type dictionary from
 * given input dictionary file name and by using turkish_finite_state_machine.xml file.
 *
 * @param fileName           the file to read the finite fsm_state machine.
 * @param dictionaryFileName the file to read the dictionary.
 */
Fsm_morphological_analyzer_ptr create_fsm_morphological_analyzer2(char *dictionaryFileName, char *fileName) {
    return create_fsm_morphological_analyzer(fileName,
                                             create_txt_dictionary3(dictionaryFileName, "turkish_misspellings.txt",
                                                                    "turkish_morphological_lexicon.txt"), 10000);
}

void free_fsm_morphological_analyzer(Fsm_morphological_analyzer_ptr fsm_morphological_analyzer) {
    free_finite_state_machine(fsm_morphological_analyzer->finite_state_machine);
    free_txt_dictionary(fsm_morphological_analyzer->dictionary);
    free_trie(fsm_morphological_analyzer->dictionary_trie);
    free_trie(fsm_morphological_analyzer->suffix_trie);
    if (fsm_morphological_analyzer->parsed_surface_forms != NULL){
        free_hash_map2(fsm_morphological_analyzer->parsed_surface_forms, free_, free_);
    }
    free_lru_cache(fsm_morphological_analyzer->cache, (void (*)(void *)) free_fsm_parse_list);
    free_hash_map(fsm_morphological_analyzer->most_used_patterns, (void (*)(void *)) free_regular_expression);
    free_(fsm_morphological_analyzer);
}

void prepare_suffix_tree(Fsm_morphological_analyzer_ptr fsm_morphological_analyzer) {
    fsm_morphological_analyzer->suffix_trie = create_trie();
    Array_list_ptr lines = read_lines("suffixes.txt");
    for (int i = 0; i < lines->size; i++) {
        char *line = array_list_get(lines, i);
        char *reverse_suffix = reverse_string(line);
        add_word_to_trie(fsm_morphological_analyzer->suffix_trie, reverse_suffix, create_txt_word(reverse_suffix));
        free_(reverse_suffix);
    }
    free_array_list(lines, free_);
}

void add_surface_forms(Fsm_morphological_analyzer_ptr fsm_morphological_analyzer, const char *file_name) {
    fsm_morphological_analyzer->parsed_surface_forms = read_hash_map(file_name);
}

/**
 * The getPossibleWords method takes MorphologicalParse and MetamorphicParse as input.
 * First it determines whether the given morphologicalParse is the root verb and whether it contains a verb tag.
 * Then it creates new transition with -mak and creates a new HashSet result.
 * <p>
 * It takes the given MetamorphicParse input as currentWord and if there is a compound word starting with the
 * currentWord, it gets this compoundWord from dictionaryTrie. If there is a compoundWord and the difference of the
 * currentWord and compundWords is less than 3 than compoundWord is added to the result, otherwise currentWord is added.
 * <p>
 * Then it gets the root from parse input as a currentRoot. If it is not null, and morphologicalParse input is verb,
 * it directly adds the verb to result after making transition to currentRoot with currentWord String. Else, it creates a new
 * transition with -lar and make this transition then adds to the result.
 *
 * @param morphologicalParse MorphologicalParse type input.
 * @param parse              MetamorphicParse type input.
 * @return HashSet result.
 */
Hash_set_ptr get_possible_words(Fsm_morphological_analyzer_ptr fsm_morphological_analyzer,
                                const Morphological_parse *morphological_parse,
                                const Metamorphic_parse *metamorphic_parse) {
    char *root_pos = get_root_pos(morphological_parse);
    bool is_root_verb = strcmp(root_pos, "VERB") == 0;
    free_(root_pos);
    bool contains_verb = parse_contains_tag(morphological_parse, VERB);
    Transition_ptr verb_transition = create_transition3("mAk");
    Txt_word_ptr compound_word;
    Txt_word_ptr current_root;
    Hash_set_ptr result = create_hash_set((unsigned int (*)(const void *, int)) hash_function_string,
                                          (int (*)(const void *, const void *)) compare_string);
    if (metamorphic_parse->root == NULL) {
        free_transition(verb_transition);
        return result;
    }
    char *verb_word, *plural_word, *current_word = metamorphic_parse->root;
    int plural_index = -1;
    compound_word = get_compound_word_starting_with(fsm_morphological_analyzer->dictionary_trie, current_word);
    if (!is_root_verb) {
        if (compound_word != NULL && word_size(compound_word->name) - word_size(current_word) < 3) {
            hash_set_insert(result, compound_word->name);
        }
        hash_set_insert(result, current_word);
    }
    current_root = get_word_txt(fsm_morphological_analyzer->dictionary, metamorphic_parse->root);
    if (current_root == NULL && compound_word != NULL) {
        current_root = compound_word;
    }
    if (current_root != NULL) {
        if (is_root_verb) {
            verb_word = make_transition(verb_transition, current_root, current_word);
            hash_set_insert(result, verb_word);
        }
        plural_word = "";
        for (int i = 1; i < metamorphic_parse->meta_morpheme_list->size; i++) {
            Transition_ptr transition = create_transition(create_fsm_state3(), get_meta_morpheme(metamorphic_parse, i),
                                                          "");
            if (strcmp(get_meta_morpheme(metamorphic_parse, i), "lAr") == 0) {
                plural_word = current_word;
                plural_index = i + 1;
            }
            current_word = make_transition(transition, current_root, current_word);
            free_transition(transition);
            hash_set_insert(result, current_word);
            if (contains_verb) {
                verb_word = make_transition(verb_transition, current_root, current_word);
                hash_set_insert(result, verb_word);
            }
        }
        if (plural_word != NULL) {
            current_word = plural_word;
            for (int i = plural_index; i < metamorphic_parse->meta_morpheme_list->size; i++) {
                Transition_ptr transition = create_transition(create_fsm_state3(),
                                                              get_meta_morpheme(metamorphic_parse, i), "");
                current_word = make_transition(transition, current_root, current_word);
                free_transition(transition);
                hash_set_insert(result, current_word);
                if (contains_verb) {
                    verb_word = make_transition(verb_transition, current_root, current_word);
                    hash_set_insert(result, verb_word);
                }
            }
        }
    }
    free_transition(verb_transition);
    return result;
}

/**
 * The isPossibleSubstring method first checks whether given short and long strings are equal to root word.
 * Then, compares both short and long strings' chars till the last two chars of short string. In the presence of mismatch,
 * false is returned. On the other hand, it counts the distance between two strings until it becomes greater than 2,
 * which is the MAX_DISTANCE also finds the index of the last char.
 * <p>
 * If the substring is a rootWord and equals to 'ben', which is a special case or root holds the lastIdropsDuringSuffixation or
 * lastIdropsDuringPassiveSuffixation conditions, then it returns true if distance is not greater than MAX_DISTANCE.
 * <p>
 * On the other hand, if the shortStrong ends with one of these chars 'e, a, p, ç, t, k' and 't 's a rootWord with
 * the conditions of rootSoftenDuringSuffixation, vowelEChangesToIDuringYSuffixation, vowelAChangesToIDuringYSuffixation
 * or endingKChangesIntoG then it returns true if the last index is not equal to 2 and distance is not greater than
 * MAX_DISTANCE and false otherwise.
 *
 * @param shortString the possible substring.
 * @param longString  the long string to compare with substring.
 * @param root        the root of the long string.
 * @return true if given substring is the actual substring of the longString, false otherwise.
 */
bool is_possible_substring(const char *shortString,
                           const char *longString,
                           Txt_word_ptr root) {
    bool rootWord = strcmp(shortString, root->name) == 0 || strcmp(longString, root->name) == 0;
    int distance = 0, j, last = 1;
    for (j = 0; j < word_size(shortString); j++) {
        String_ptr ch1 = char_at(shortString, j);
        String_ptr ch2 = char_at(longString, j);
        if (!string_equals(ch1, ch2)) {
            free_string_ptr(ch1);
            free_string_ptr(ch2);
            if (j < word_size(shortString) - 2) {
                return false;
            }
            last = word_size(shortString) - j;
            distance++;
            if (distance > MAX_DISTANCE) {
                break;
            }
        } else {
            free_string_ptr(ch1);
            free_string_ptr(ch2);
        }
    }
    if (rootWord &&
        (string_in_list(root->name, (char *[]) {"ben", "sen"}, 2) || last_i_drops_during_suffixation(root) ||
         last_i_drops_during_passive_suffixation(root))) {
        return (distance <= MAX_DISTANCE);
    } else {
        String_ptr ch = last_char(shortString);
        if (string_in_list(ch->s, (char *[]) {"e", "a", "p", "ç", "t", "k"}, 6) ||
            (rootWord && (root_soften_during_suffixation(root) ||
                          vowel_e_changes_to_i_during_y_suffixation(root) ||
                          vowel_a_changes_to_i_during_y_suffixation(root) ||
                          ending_k_changes_into_g(root)))) {
            free_string_ptr(ch);
            return (last != 2 && distance <= MAX_DISTANCE - 1);
        } else {
            free_string_ptr(ch);
            return (distance <= MAX_DISTANCE - 2);
        }
    }
}

/**
 * The initializeParseList method initializes the given given fsm ArrayList with given root words by parsing them.
 * <p>
 * It checks many conditions;
 * isPlural; if root holds the condition then it gets the fsm_state with the name of NominalRootPlural, then
 * creates a new parsing and adds this to the input fsm_parse Arraylist.
 * Ex : Açıktohumlular
 * <p>
 * !isPlural and isPortmanteauEndingWithSI, if root holds the conditions then it gets the fsm_state with the
 * name of NominalRootNoPossesive.
 * Ex : Balarısı
 * <p>
 * !isPlural and isPortmanteau, if root holds the conditions then it gets the fsm_state with the name of
 * CompoundNounRoot.
 * Ex : Aslanağızı
 * <p>
 * !isPlural, !isPortmanteau and isHeader, if root holds the conditions then it gets the fsm_state with the
 * name of HeaderRoot.
 * Ex :  </title>
 * <p>
 * !isPlural, !isPortmanteau and isInterjection, if root holds the conditions then it gets the fsm_state
 * with the name of InterjectionRoot.
 * Ex : Hey, Aa
 * <p>
 * !isPlural, !isPortmanteau and isDuplicate, if root holds the conditions then it gets the fsm_state
 * with the name of DuplicateRoot.
 * Ex : Allak,
 * <p>
 * !isPlural, !isPortmanteau and isCode, if root holds the conditions then it gets the fsm_state
 * with the name of CodeRoot.
 * Ex : 9400f,
 * <p>
 * !isPlural, !isPortmanteau and isMetric, if root holds the conditions then it gets the fsm_state
 * with the name of MetricRoot.
 * Ex : 11x8x12,
 * <p>
 * !isPlural, !isPortmanteau and isNumeral, if root holds the conditions then it gets the fsm_state
 * with the name of CardinalRoot.
 * Ex : Yüz, bin
 * <p>
 * !isPlural, !isPortmanteau and isReal, if root holds the conditions then it gets the fsm_state
 * with the name of RealRoot.
 * Ex : 1.2
 * <p>
 * !isPlural, !isPortmanteau and isFraction, if root holds the conditions then it gets the fsm_state
 * with the name of FractionRoot.
 * Ex : 1/2
 * <p>
 * !isPlural, !isPortmanteau and isDate, if root holds the conditions then it gets the fsm_state
 * with the name of DateRoot.
 * Ex : 11/06/2018
 * <p>
 * !isPlural, !isPortmanteau and isPercent, if root holds the conditions then it gets the fsm_state
 * with the name of PercentRoot.
 * Ex : %12.5
 * <p>
 * !isPlural, !isPortmanteau and isRange, if root holds the conditions then it gets the fsm_state
 * with the name of RangeRoot.
 * Ex : 3-5
 * <p>
 * !isPlural, !isPortmanteau and isTime, if root holds the conditions then it gets the fsm_state
 * with the name of TimeRoot.
 * Ex : 13:16:08
 * <p>
 * !isPlural, !isPortmanteau and isOrdinal, if root holds the conditions then it gets the fsm_state
 * with the name of OrdinalRoot.
 * Ex : Altıncı
 * <p>
 * !isPlural, !isPortmanteau, and isVerb if root holds the conditions then it gets the fsm_state
 * with the name of VerbalRoot. Or isPassive, then it gets the fsm_state with the name of PassiveHn.
 * Ex : Anla (!isPAssive)
 * Ex : Çağrıl (isPassive)
 * <p>
 * !isPlural, !isPortmanteau and isPronoun, if root holds the conditions then it gets the fsm_state
 * with the name of PronounRoot. There are 6 different Pronoun fsm_state names, REFLEX, QUANT, QUANTPLURAL, DEMONS, PERS, QUES.
 * REFLEX = Reflexive Pronouns Ex : kendi
 * QUANT = Quantitative Pronouns Ex : öbür, hep, kimse, hiçbiri, bazı, kimi, biri
 * QUANTPLURAL = Quantitative Plural Pronouns Ex : tümü, çoğu, hepsi
 * DEMONS = Demonstrative Pronouns Ex : o, bu, şu
 * PERS = Personal Pronouns Ex : ben, sen, o, biz, siz, onlar
 * QUES = Interrogatıve Pronouns Ex : nere, ne, kim, hangi
 * <p>
 * !isPlural, !isPortmanteau and isAdjective, if root holds the conditions then it gets the fsm_state
 * with the name of AdjectiveRoot.
 * Ex : Absürt, Abes
 * <p>
 * !isPlural, !isPortmanteau and isPureAdjective, if root holds the conditions then it gets the fsm_state
 * with the name of Adjective.
 * Ex : Geçmiş, Cam
 * <p>
 * !isPlural, !isPortmanteau and isNominal, if root holds the conditions then it gets the fsm_state
 * with the name of NominalRoot.
 * Ex : Görüş
 * <p>
 * !isPlural, !isPortmanteau and is_proper, if root holds the conditions then it gets the fsm_state
 * with the name of ProperRoot.
 * Ex : Abdi
 * <p>
 * !isPlural, !isPortmanteau and isQuestion, if root holds the conditions then it gets the fsm_state
 * with the name of QuestionRoot.
 * Ex : Mi, mü
 * <p>
 * !isPlural, !isPortmanteau and isDeterminer, if root holds the conditions then it gets the fsm_state
 * with the name of DeterminerRoot.
 * Ex : Çok, bir
 * <p>
 * !isPlural, !isPortmanteau and isConjunction, if root holds the conditions then it gets the fsm_state
 * with the name of ConjunctionRoot.
 * Ex : Ama , ancak
 * <p>
 * !isPlural, !isPortmanteau and isPostP, if root holds the conditions then it gets the fsm_state
 * with the name of PostP.
 * Ex : Ait, dair
 * <p>
 * !isPlural, !isPortmanteau and isAdverb, if root holds the conditions then it gets the fsm_state
 * with the name of AdverbRoot.
 * Ex : Acilen
 *
 * @param fsm_parse ArrayList to initialize.
 * @param root     word to check properties and add to fsm_parse according to them.
 * @param is_proper is used to check a word is proper or not.
 */
void initialize_parse_list(Fsm_morphological_analyzer_ptr fsm_morphological_analyzer,
                           Array_list_ptr fsm_parse,
                           Txt_word_ptr root,
                           bool is_proper) {
    Fsm_parse_ptr current_fsm_parse;
    if (is_plural(root)) {
        current_fsm_parse = create_fsm_parse6(root, get_state(fsm_morphological_analyzer->finite_state_machine,
                                                              "NominalRootPlural"));
        array_list_add(fsm_parse, current_fsm_parse);
    } else {
        if (is_portmanteau_ending_with_si(root)) {
            String_ptr st = substring_except_last_two_chars(root->name);
            current_fsm_parse = create_fsm_parse5(st->s, get_state(fsm_morphological_analyzer->finite_state_machine,
                                                                   "CompoundNounRoot"));
            free_string_ptr(st);
            array_list_add(fsm_parse, current_fsm_parse);
            current_fsm_parse = create_fsm_parse6(root, get_state(fsm_morphological_analyzer->finite_state_machine,
                                                                  "NominalRootNoPossesive"));
            array_list_add(fsm_parse, current_fsm_parse);
        } else {
            if (is_portmanteau(root)) {
                if (is_portmanteau_faced_vowel_ellipsis(root)) {
                    current_fsm_parse = create_fsm_parse6(root,
                                                          get_state(fsm_morphological_analyzer->finite_state_machine,
                                                                    "NominalRootNoPossesive"));
                    array_list_add(fsm_parse, current_fsm_parse);
                    String_ptr st = substring_except_last_two_chars(root->name);
                    String_ptr st1 = last_char(root->name);
                    String_ptr st2 = char_at(root->name, word_size(root->name) - 2);
                    string_append_s(st, st1);
                    string_append_s(st, st2);
                    current_fsm_parse = create_fsm_parse5(st->s,
                                                          get_state(fsm_morphological_analyzer->finite_state_machine,
                                                                    "CompoundNounRoot"));
                    free_string_ptr(st);
                    free_string_ptr(st1);
                    free_string_ptr(st2);
                } else {
                    if (is_portmanteau_faced_softening(root)) {
                        current_fsm_parse = create_fsm_parse6(root, get_state(
                                fsm_morphological_analyzer->finite_state_machine, "NominalRootNoPossesive"));
                        array_list_add(fsm_parse, current_fsm_parse);
                        String_ptr lastBefore = char_at(root->name, word_size(root->name) - 2);
                        if (string_equals2(lastBefore, "b")) {
                            String_ptr st = substring_except_last_two_chars(root->name);
                            string_append(st, "p");
                            current_fsm_parse = create_fsm_parse5(st->s, get_state(
                                    fsm_morphological_analyzer->finite_state_machine, "CompoundNounRoot"));
                            free_string_ptr(st);
                        } else {
                            if (string_equals2(lastBefore, "c")) {
                                String_ptr st = substring_except_last_two_chars(root->name);
                                string_append(st, "ç");
                                current_fsm_parse = create_fsm_parse5(st->s, get_state(
                                        fsm_morphological_analyzer->finite_state_machine, "CompoundNounRoot"));
                                free_string_ptr(st);
                            } else {
                                if (string_equals2(lastBefore, "d")) {
                                    String_ptr st = substring_except_last_two_chars(root->name);
                                    string_append(st, "t");
                                    current_fsm_parse = create_fsm_parse5(st->s, get_state(
                                            fsm_morphological_analyzer->finite_state_machine, "CompoundNounRoot"));
                                    free_string_ptr(st);
                                } else {
                                    if (string_equals2(lastBefore, "ğ")) {
                                        String_ptr st = substring_except_last_two_chars(root->name);
                                        string_append(st, "k");
                                        current_fsm_parse = create_fsm_parse5(st->s, get_state(
                                                fsm_morphological_analyzer->finite_state_machine, "CompoundNounRoot"));
                                        free_string_ptr(st);
                                    } else {
                                        String_ptr st = substring_except_last_char(root->name);
                                        current_fsm_parse = create_fsm_parse5(st->s, get_state(
                                                fsm_morphological_analyzer->finite_state_machine, "CompoundNounRoot"));
                                        free_string_ptr(st);
                                    }
                                }
                            }
                        }
                        free_string_ptr(lastBefore);
                    } else {
                        String_ptr st = substring_except_last_char(root->name);
                        current_fsm_parse = create_fsm_parse5(st->s, get_state(
                                fsm_morphological_analyzer->finite_state_machine, "CompoundNounRoot"));
                        free_string_ptr(st);
                    }
                }
                array_list_add(fsm_parse, current_fsm_parse);
            } else {
                if (is_header(root)) {
                    current_fsm_parse = create_fsm_parse6(root,
                                                          get_state(fsm_morphological_analyzer->finite_state_machine,
                                                                    "HeaderRoot"));
                    array_list_add(fsm_parse, current_fsm_parse);
                }
                if (is_interjection(root)) {
                    current_fsm_parse = create_fsm_parse6(root,
                                                          get_state(fsm_morphological_analyzer->finite_state_machine,
                                                                    "InterjectionRoot"));
                    array_list_add(fsm_parse, current_fsm_parse);
                }
                if (is_duplicate(root)) {
                    current_fsm_parse = create_fsm_parse6(root,
                                                          get_state(fsm_morphological_analyzer->finite_state_machine,
                                                                    "DuplicateRoot"));
                    array_list_add(fsm_parse, current_fsm_parse);
                }
                if (is_code(root)) {
                    current_fsm_parse = create_fsm_parse6(root,
                                                          get_state(fsm_morphological_analyzer->finite_state_machine,
                                                                    "CodeRoot"));
                    array_list_add(fsm_parse, current_fsm_parse);
                }
                if (is_metric(root)) {
                    current_fsm_parse = create_fsm_parse6(root,
                                                          get_state(fsm_morphological_analyzer->finite_state_machine,
                                                                    "MetricRoot"));
                    array_list_add(fsm_parse, current_fsm_parse);
                }
                if (is_numeral(root)) {
                    current_fsm_parse = create_fsm_parse6(root,
                                                          get_state(fsm_morphological_analyzer->finite_state_machine,
                                                                    "CardinalRoot"));
                    array_list_add(fsm_parse, current_fsm_parse);
                }
                if (is_real(root)) {
                    current_fsm_parse = create_fsm_parse6(root,
                                                          get_state(fsm_morphological_analyzer->finite_state_machine,
                                                                    "RealRoot"));
                    array_list_add(fsm_parse, current_fsm_parse);
                }
                if (is_fraction(root)) {
                    current_fsm_parse = create_fsm_parse6(root,
                                                          get_state(fsm_morphological_analyzer->finite_state_machine,
                                                                    "FractionRoot"));
                    array_list_add(fsm_parse, current_fsm_parse);
                }
                if (is_date(root)) {
                    current_fsm_parse = create_fsm_parse6(root,
                                                          get_state(fsm_morphological_analyzer->finite_state_machine,
                                                                    "DateRoot"));
                    array_list_add(fsm_parse, current_fsm_parse);
                }
                if (is_percent(root)) {
                    current_fsm_parse = create_fsm_parse6(root,
                                                          get_state(fsm_morphological_analyzer->finite_state_machine,
                                                                    "PercentRoot"));
                    array_list_add(fsm_parse, current_fsm_parse);
                }
                if (is_range(root)) {
                    current_fsm_parse = create_fsm_parse6(root,
                                                          get_state(fsm_morphological_analyzer->finite_state_machine,
                                                                    "RangeRoot"));
                    array_list_add(fsm_parse, current_fsm_parse);
                }
                if (is_time2(root)) {
                    current_fsm_parse = create_fsm_parse6(root,
                                                          get_state(fsm_morphological_analyzer->finite_state_machine,
                                                                    "TimeRoot"));
                    array_list_add(fsm_parse, current_fsm_parse);
                }
                if (is_ordinal(root)) {
                    current_fsm_parse = create_fsm_parse6(root,
                                                          get_state(fsm_morphological_analyzer->finite_state_machine,
                                                                    "OrdinalRoot"));
                    array_list_add(fsm_parse, current_fsm_parse);
                }
                if (is_verb(root) || is_passive(root)) {
                    if (verb_type(root) != NULL) {
                        String_ptr st = create_string4("VerbalRoot(", verb_type(root), ")");
                        current_fsm_parse = create_fsm_parse6(root, get_state(
                                fsm_morphological_analyzer->finite_state_machine, st->s));
                        free_string_ptr(st);
                    } else {
                        if (!is_passive(root)) {
                            current_fsm_parse = create_fsm_parse6(root, get_state(
                                    fsm_morphological_analyzer->finite_state_machine, "VerbalRoot"));
                        } else {
                            current_fsm_parse = create_fsm_parse6(root, get_state(
                                    fsm_morphological_analyzer->finite_state_machine, "PassiveHn"));
                        }
                    }
                    array_list_add(fsm_parse, current_fsm_parse);
                }
                if (is_pronoun(root)) {
                    if (strcmp(root->name, "kendi") == 0) {
                        current_fsm_parse = create_fsm_parse6(root, get_state(
                                fsm_morphological_analyzer->finite_state_machine, "PronounRoot(REFLEX)"));
                        array_list_add(fsm_parse, current_fsm_parse);
                    }
                    if (string_in_list(root->name,
                                       (char *[]) {"öbür", "öteki", "hep", "kimse", "diğeri", "hiçbiri", "böylesi",
                                                   "birbiri", "birbirleri", "biri", "başkası", "bazı", "kimi"}, 13)) {
                        current_fsm_parse = create_fsm_parse6(root, get_state(
                                fsm_morphological_analyzer->finite_state_machine, "PronounRoot(QUANT)"));
                        array_list_add(fsm_parse, current_fsm_parse);
                    }
                    if (string_in_list(root->name,
                                       (char *[]) {"tümü", "topu", "herkes", "cümlesi", "çoğu", "birçoğu", "birkaçı",
                                                   "birçokları", "hepsi"}, 9)) {
                        current_fsm_parse = create_fsm_parse6(root, get_state(
                                fsm_morphological_analyzer->finite_state_machine, "PronounRoot(QUANTPLURAL)"));
                        array_list_add(fsm_parse, current_fsm_parse);
                    }
                    if (string_in_list(root->name, (char *[]) {"o", "bu", "şu"}, 3)) {
                        current_fsm_parse = create_fsm_parse6(root, get_state(
                                fsm_morphological_analyzer->finite_state_machine, "PronounRoot(DEMONS)"));
                        array_list_add(fsm_parse, current_fsm_parse);
                    }
                    if (string_in_list(root->name, (char *[]) {"ben", "sen", "o", "biz", "siz", "onlar"}, 6)) {
                        current_fsm_parse = create_fsm_parse6(root, get_state(
                                fsm_morphological_analyzer->finite_state_machine, "PronounRoot(PERS)"));
                        array_list_add(fsm_parse, current_fsm_parse);
                    }
                    if (string_in_list(root->name, (char *[]) {"nere", "ne", "kaçı", "kim", "hangi"}, 5)) {
                        current_fsm_parse = create_fsm_parse6(root, get_state(
                                fsm_morphological_analyzer->finite_state_machine, "PronounRoot(QUES)"));
                        array_list_add(fsm_parse, current_fsm_parse);
                    }
                }
                if (is_adjective(root)) {
                    current_fsm_parse = create_fsm_parse6(root,
                                                          get_state(fsm_morphological_analyzer->finite_state_machine,
                                                                    "AdjectiveRoot"));
                    array_list_add(fsm_parse, current_fsm_parse);
                }
                if (is_pure_adjective(root)) {
                    current_fsm_parse = create_fsm_parse6(root,
                                                          get_state(fsm_morphological_analyzer->finite_state_machine,
                                                                    "Adjective"));
                    array_list_add(fsm_parse, current_fsm_parse);
                }
                if (is_nominal(root)) {
                    current_fsm_parse = create_fsm_parse6(root,
                                                          get_state(fsm_morphological_analyzer->finite_state_machine,
                                                                    "NominalRoot"));
                    array_list_add(fsm_parse, current_fsm_parse);
                }
                if (is_abbreviation(root)) {
                    current_fsm_parse = create_fsm_parse6(root,
                                                          get_state(fsm_morphological_analyzer->finite_state_machine,
                                                                    "NominalRoot"));
                    array_list_add(fsm_parse, current_fsm_parse);
                }
                if (is_proper_noun(root) && is_proper) {
                    current_fsm_parse = create_fsm_parse6(root,
                                                          get_state(fsm_morphological_analyzer->finite_state_machine,
                                                                    "ProperRoot"));
                    array_list_add(fsm_parse, current_fsm_parse);
                }
                if (is_question(root)) {
                    current_fsm_parse = create_fsm_parse6(root,
                                                          get_state(fsm_morphological_analyzer->finite_state_machine,
                                                                    "QuestionRoot"));
                    array_list_add(fsm_parse, current_fsm_parse);
                }
                if (is_determiner(root)) {
                    current_fsm_parse = create_fsm_parse6(root,
                                                          get_state(fsm_morphological_analyzer->finite_state_machine,
                                                                    "DeterminerRoot"));
                    array_list_add(fsm_parse, current_fsm_parse);
                }
                if (is_conjunction(root)) {
                    current_fsm_parse = create_fsm_parse6(root,
                                                          get_state(fsm_morphological_analyzer->finite_state_machine,
                                                                    "ConjunctionRoot"));
                    array_list_add(fsm_parse, current_fsm_parse);
                }
                if (is_postp(root)) {
                    current_fsm_parse = create_fsm_parse6(root,
                                                          get_state(fsm_morphological_analyzer->finite_state_machine,
                                                                    "PostP"));
                    array_list_add(fsm_parse, current_fsm_parse);
                }
                if (is_adverb(root)) {
                    current_fsm_parse = create_fsm_parse6(root,
                                                          get_state(fsm_morphological_analyzer->finite_state_machine,
                                                                    "AdverbRoot"));
                    array_list_add(fsm_parse, current_fsm_parse);
                }
            }
        }
    }
}

/**
 * The initializeParseListFromRoot method is used to create an ArrayList which consists of initial fsm parsings. First, traverses
 * this HashSet and uses each word as a root and calls initializeParseList method with this root and ArrayList.
 * <p>
 *
 * @param parseList ArrayList to initialize.
 * @param root the root form to generate initial parse list.
 * @param isProper    is used to check a word is proper or not.
 */
void initialize_parse_list_from_root(Fsm_morphological_analyzer_ptr fsm_morphological_analyzer,
                                     Array_list_ptr parse_list,
                                     Txt_word_ptr root,
                                     bool is_proper) {
    initialize_parse_list(fsm_morphological_analyzer, parse_list, root, is_proper);
    if (obeys_and_not_obeys_vowel_harmony_during_agglutination(root)) {
        Txt_word_ptr new_root = clone_txt_word(root);
        remove_flag(new_root, "IS_UU");
        remove_flag(new_root, "IS_UUU");
        initialize_parse_list(fsm_morphological_analyzer, parse_list, new_root, is_proper);
        free_txt_word(new_root);
    }
    if (root_soften_and_not_soften_during_suffixation(root)) {
        Txt_word_ptr new_root = clone_txt_word(root);
        remove_flag(new_root, "IS_SD");
        remove_flag(new_root, "IS_SDD");
        initialize_parse_list(fsm_morphological_analyzer, parse_list, new_root, is_proper);
        free_txt_word(new_root);
    }
    if (last_i_drops_and_not_drop_during_suffixation(root)) {
        Txt_word_ptr new_root = clone_txt_word(root);
        remove_flag(new_root, "IS_UD");
        remove_flag(new_root, "IS_UDD");
        initialize_parse_list(fsm_morphological_analyzer, parse_list, new_root, is_proper);
        free_txt_word(new_root);
    }
    if (duplicates_and_not_duplicates_during_suffixation(root)) {
        Txt_word_ptr new_root = clone_txt_word(root);
        remove_flag(new_root, "IS_ST");
        remove_flag(new_root, "IS_STT");
        initialize_parse_list(fsm_morphological_analyzer, parse_list, new_root, is_proper);
        free_txt_word(new_root);
    }
    if (ending_k_changes_into_g(root) && contains_flag(root, "IS_OA")) {
        Txt_word_ptr new_root = clone_txt_word(root);
        remove_flag(new_root, "IS_OA");
        initialize_parse_list(fsm_morphological_analyzer, parse_list, new_root, is_proper);
        free_txt_word(new_root);
    }
}

/**
 * The initializeRootList method is used to create an ArrayList which consists of initial fsm parsings. First,
 * it calls getWordsWithPrefix methods by using input String surfaceForm and generates a HashSet. Then, traverses
 * this HashSet and uses each word as a root and calls initializeParseList method with this root and ArrayList.
 * <p>
 *
 * @param surfaceForm the String used to generate a HashSet of words.
 * @param is_proper    is used to check a word is proper or not.
 * @return initialFsmParse ArrayList.
 */
Array_list_ptr initialize_parse_list_from_surface_form(Fsm_morphological_analyzer_ptr fsm_morphological_analyzer,
                                                       const char *surface_form,
                                                       bool is_proper) {
    Txt_word_ptr root;
    Array_list_ptr initial_fsm_parse = create_array_list();
    if (surface_form == NULL) {
        return initial_fsm_parse;
    }
    Hash_set_ptr words = get_words_with_prefix(fsm_morphological_analyzer->dictionary_trie, surface_form);
    Array_list_ptr list = hash_set_key_list(words);
    for (int i = 0; i < list->size; i++) {
        root = array_list_get(list, i);
        initialize_parse_list_from_root(fsm_morphological_analyzer, initial_fsm_parse, root, is_proper);
    }
    free_hash_set(words, (void (*)(void *)) free_txt_word);
    free_array_list(list, NULL);
    return initial_fsm_parse;
}

/**
 * The addNewParsesFromCurrentParse method initially gets the final suffixes from input current_fsm_parse called as currentState,
 * and by using the currentState information it gets the new analysis. Then loops through each currentState's transition.
 * If the currentTransition is possible, it makes the transition
 *
 * @param current_fsm_parse FsmParse type input.
 * @param fsm_parse_queue        an ArrayList of FsmParse.
 * @param surfaceForm     String to use during transition.
 * @param root            TxtWord used to make transition.
 */
void add_new_parses_from_current_parse(Fsm_morphological_analyzer_ptr fsm_morphological_analyzer,
                                       Fsm_parse_ptr current_fsm_parse,
                                       Queue_ptr fsm_parse_queue,
                                       int max_length,
                                       Txt_word_ptr root) {
    Fsm_State_ptr current_state = get_final_suffix(current_fsm_parse);
    char *current_surface_form = current_fsm_parse->form;
    Array_list_ptr transitions = get_transitions(fsm_morphological_analyzer->finite_state_machine, current_state);
    if (transitions->size == 0){
        free_array_list(transitions, NULL);
        return;
    }
    for (int i = 0; i < transitions->size; i++) {
        Transition_ptr current_transition = array_list_get(transitions, i);
        if (transition_possible2(current_transition, current_fsm_parse) &&
            (strcmp(current_surface_form, root->name) != 0 || (strcmp(current_surface_form, root->name) == 0 &&
                                                                     transition_possible3(current_transition, root,
                                                                                          current_state)))) {
            char *tmp = make_transition2(current_transition, root, current_surface_form,
                                         get_start_state(current_fsm_parse));
            if (word_size(tmp) <= max_length) {
                Fsm_parse_ptr new_fsm_parse = clone_fsm_parse(current_fsm_parse);
                add_suffix(new_fsm_parse, current_transition->to_state, tmp, current_transition->with_name,
                           current_transition->with, current_transition->to_pos);
                set_agreement(new_fsm_parse, current_transition->with_name);
                enqueue(fsm_parse_queue, new_fsm_parse);
            }
            free_(tmp);
        }
    }
}

/**
 * The addNewParsesFromCurrentParse method initially gets the final suffixes from input current_fsm_parse called as currentState,
 * and by using the currentState information it gets the currentSurfaceForm. Then loops through each currentState's transition.
 * If the currentTransition is possible, it makes the transition
 *
 * @param current_fsm_parse FsmParse type input.
 * @param fsm_parse_queue        an ArrayList of FsmParse.
 * @param surface_form     String to use during transition.
 * @param root            TxtWord used to make transition.
 */
void add_new_parses_from_current_parse2(Fsm_morphological_analyzer_ptr fsm_morphological_analyzer,
                                        Fsm_parse_ptr current_fsm_parse,
                                        Queue_ptr fsm_parse_queue,
                                        char *surface_form,
                                        Txt_word_ptr root) {
    Fsm_State_ptr current_state = get_final_suffix(current_fsm_parse);
    char *current_surface_form = current_fsm_parse->form;
    Array_list_ptr transitions = get_transitions(fsm_morphological_analyzer->finite_state_machine, current_state);
    if (transitions->size == 0){
        free_array_list(transitions, NULL);
        return;
    }
    for (int i = 0; i < transitions->size; i++) {
        Transition_ptr currentTransition = array_list_get(transitions, i);
        if (transition_possible1(currentTransition, current_fsm_parse->form, surface_form) &&
            transition_possible2(currentTransition, current_fsm_parse) &&
            (strcmp(current_surface_form, root->name) != 0 || (strcmp(current_surface_form, root->name) == 0 &&
                                                                     transition_possible3(currentTransition, root,
                                                                                        current_state)))) {
            char *tmp = make_transition2(currentTransition, root, current_surface_form,
                                         get_start_state(current_fsm_parse));
            if ((word_size(tmp) < word_size(surface_form) && is_possible_substring(tmp, surface_form, root)) ||
                (word_size(tmp) == word_size(surface_form) &&
                 (last_i_drops_during_suffixation(root) || strcmp(tmp, surface_form) == 0))) {
                Fsm_parse_ptr new_fsm_parse = clone_fsm_parse(current_fsm_parse);
                add_suffix(new_fsm_parse, currentTransition->to_state, tmp, currentTransition->with_name, currentTransition->with, currentTransition->to_pos);
                set_agreement(new_fsm_parse, currentTransition->with_name);
                enqueue(fsm_parse_queue, new_fsm_parse);
            }
            free_(tmp);
        }
    }
}

/**
 * The parseExists method is used to check the existence of the parse.
 *
 * @param fsm_parses    an ArrayList of FsmParse
 * @param surfaceForm String to use during transition.
 * @return true when the currentState is end fsm_state and input surfaceForm id equal to currentSurfaceForm, otherwise false.
 */
bool
parse_exists(Fsm_morphological_analyzer_ptr fsm_morphological_analyzer, Array_list_ptr fsm_parses, char *surface_form) {
    Fsm_parse_ptr current_fsm_parse;
    Txt_word_ptr root;
    Fsm_State_ptr current_state;
    char *current_surface_form;
    Queue_ptr parse_queue = create_queue2(fsm_parses);
    while (!is_queue_empty(parse_queue)) {
        current_fsm_parse = dequeue(parse_queue);
        root = current_fsm_parse->root;
        current_state = get_final_suffix(current_fsm_parse);
        current_surface_form = current_fsm_parse->form;
        if (current_state->end_state && strcmp(current_surface_form, surface_form) == 0) {
            free_queue(parse_queue, (void (*)(void *)) free_fsm_parse);
            return true;
        }
        add_new_parses_from_current_parse2(fsm_morphological_analyzer, current_fsm_parse, parse_queue, surface_form,
                                           root);
        free_fsm_parse(current_fsm_parse);
    }
    free_queue(parse_queue, (void (*)(void *)) free_fsm_parse);
    return false;
}

/**
 * The parseWord method is used to parse a given fsm_parses. It simply adds new parses to the current parse by
 * using addNewParsesFromCurrentParse method.
 *
 * @param fsm_parses    an ArrayList of FsmParse
 * @param max_length maximum length of the surfaceform.
 * @return result ArrayList which has the currentFsmParse.
 */
Array_list_ptr
parse_word(Fsm_morphological_analyzer_ptr fsm_morphological_analyzer, Array_list_ptr fsm_parses, int max_length) {
    Array_list_ptr result = create_array_list();
    Array_list_ptr result_suffix_list = create_array_list();
    Fsm_parse_ptr current_fsm_parse;
    Txt_word_ptr root;
    Fsm_State_ptr current_state;
    bool is_a_correct_parse;
    char *current_surface_form;
    char *current_suffix_list;
    Queue_ptr parse_queue = create_queue2(fsm_parses);
    while (!is_queue_empty(parse_queue)) {
        current_fsm_parse = dequeue(parse_queue);
        is_a_correct_parse = false;
        root = current_fsm_parse->root;
        current_state = get_final_suffix(current_fsm_parse);
        current_surface_form = current_fsm_parse->form;
        if (current_state->end_state && word_size(current_surface_form) <= max_length) {
            current_suffix_list = get_suffix_list(current_fsm_parse);
            if (!array_list_contains(result_suffix_list, current_suffix_list,
                                     (int (*)(const void *, const void *)) compare_string)) {
                is_a_correct_parse = true;
                construct_inflectional_groups(current_fsm_parse);
                array_list_add(result, current_fsm_parse);
                array_list_add(result_suffix_list, current_suffix_list);
            } else {
                free_(current_suffix_list);
            }
        }
        add_new_parses_from_current_parse(fsm_morphological_analyzer, current_fsm_parse, parse_queue, max_length, root);
        if (!is_a_correct_parse){
            free_fsm_parse(current_fsm_parse);
        }
    }
    free_array_list(result_suffix_list, free_);
    free_queue(parse_queue, (void (*)(void *)) free_fsm_parse);
    return result;
}

/**
 * The parseWord method is used to parse a given fsmParse. It simply adds new parses to the current parse by
 * using addNewParsesFromCurrentParse method.
 *
 * @param fsmParse    an ArrayList of FsmParse
 * @param surface_form String to use during transition.
 * @return result ArrayList which has the currentFsmParse.
 */
Array_list_ptr
parse_word2(Fsm_morphological_analyzer_ptr fsm_morphological_analyzer, Array_list_ptr fsm_parses, char *surface_form) {
    Array_list_ptr result = create_array_list();
    Array_list_ptr result_suffix_list = create_array_list();
    Fsm_parse_ptr current_fsm_parse;
    Txt_word_ptr root;
    Fsm_State_ptr current_state;
    bool is_a_correct_parse;
    char *current_surface_form;
    char *current_suffix_list;
    Queue_ptr parse_queue = create_queue2(fsm_parses);
    while (!is_queue_empty(parse_queue)) {
        current_fsm_parse = dequeue(parse_queue);
        is_a_correct_parse = false;
        root = current_fsm_parse->root;
        current_state = get_final_suffix(current_fsm_parse);
        current_surface_form = current_fsm_parse->form;
        if (current_state->end_state && strcmp(current_surface_form, surface_form) == 0) {
            current_suffix_list = get_suffix_list(current_fsm_parse);
            if (!array_list_contains(result_suffix_list, current_suffix_list,
                                     (int (*)(const void *, const void *)) compare_string)) {
                is_a_correct_parse = true;
                construct_inflectional_groups(current_fsm_parse);
                array_list_add(result, current_fsm_parse);
                array_list_add(result_suffix_list, current_suffix_list);
            } else {
                free_(current_suffix_list);
            }
        }
        add_new_parses_from_current_parse2(fsm_morphological_analyzer, current_fsm_parse, parse_queue, surface_form, root);
        if (!is_a_correct_parse){
            free_fsm_parse(current_fsm_parse);
        }
    }
    free_array_list(result_suffix_list, free_);
    free_queue(parse_queue, (void (*)(void *)) free_fsm_parse);
    return result;
}

/**
 * The morphologicalAnalysis with 3 inputs is used to initialize an ArrayList and add a new FsmParse
 * with given root and fsm_state.
 *
 * @param root        TxtWord input.
 * @param surface_form String input to use for parsing.
 * @param state       String input.
 * @return parseWord method with newly populated FsmParse ArrayList and input surface_form.
 */
Array_list_ptr morphological_analysis2(Fsm_morphological_analyzer_ptr fsm_morphological_analyzer,
                                       Txt_word_ptr root,
                                       char *surface_form,
                                       char *state) {
    Array_list_ptr initial_fsm_parses = create_array_list();
    array_list_add(initial_fsm_parses,
                   create_fsm_parse6(root, get_state(fsm_morphological_analyzer->finite_state_machine, state)));
    Array_list_ptr result = parse_word2(fsm_morphological_analyzer, initial_fsm_parses, surface_form);
    free_array_list(initial_fsm_parses, NULL);
    return result;
}

/**
 * The generateAllParses with 2 inputs is used to generate all parses with given root. Then it calls initializeParseListFromRoot method to initialize list with newly created ArrayList, input root,
 * and maximum length.
 *
 * @param root        TxtWord input.
 * @param max_length Maximum length of the surface form.
 * @return parseWord method with newly populated FsmParse ArrayList and maximum length.
 */
Array_list_ptr
generate_all_parses(Fsm_morphological_analyzer_ptr fsm_morphological_analyzer, Txt_word_ptr root, int max_length) {
    Array_list_ptr initial_fsm_parses = create_array_list();
    if (is_proper_noun(root)) {
        initialize_parse_list_from_root(fsm_morphological_analyzer, initial_fsm_parses, root, true);
    }
    initialize_parse_list_from_root(fsm_morphological_analyzer, initial_fsm_parses, root, false);
    Array_list_ptr result = parse_word(fsm_morphological_analyzer, initial_fsm_parses, max_length);
    free_array_list(initial_fsm_parses, NULL);
    return result;
}

/**
 * The morphologicalAnalysis with 2 inputs is used to initialize an ArrayList and add a new FsmParse
 * with given root. Then it calls initializeParseList method to initialize list with newly created ArrayList, input root,
 * and input surfaceForm.
 *
 * @param root        TxtWord input.
 * @param surfaceForm String input to use for parsing.
 * @return parseWord method with newly populated FsmParse ArrayList and input surfaceForm.
 */
Array_list_ptr morphological_analysis3(Fsm_morphological_analyzer_ptr fsm_morphological_analyzer, Txt_word_ptr root,
                                       char *surface_form) {
    Array_list_ptr initial_fsm_parses = create_array_list();
    initialize_parse_list_from_root(fsm_morphological_analyzer, initial_fsm_parses, root,
                                    is_proper_noun_fsm(surface_form));
    Array_list_ptr result = parse_word2(fsm_morphological_analyzer, initial_fsm_parses, surface_form);
    free_array_list(initial_fsm_parses, NULL);
    return result;
}

bool pattern_matches(Fsm_morphological_analyzer_ptr fsm_morphological_analyzer, char *expr, char *value) {
    Regular_expression_ptr r;
    if (!hash_map_contains(fsm_morphological_analyzer->most_used_patterns, expr)) {
        r = create_regular_expression(expr);
        hash_map_insert(fsm_morphological_analyzer->most_used_patterns, expr, r);
    } else {
        r = hash_map_get(fsm_morphological_analyzer->most_used_patterns, expr);
    }
    return full_matches(r, value);
}

/**
 * The isProperNoun method takes surfaceForm String as input and checks its each char whether they are in the range
 * of letters between A to Z or one of the Turkish letters such as İ, Ü, Ğ, Ş, Ç, and Ö.
 *
 * @param surfaceForm String to check for proper noun.
 * @return false if surfaceForm is null or length of 0, return true if it is a letter.
 */
bool is_proper_noun_fsm(const char *surfaceForm) {
    if (surfaceForm == NULL) {
        return false;
    }
    String_ptr st = char_at(surfaceForm, 0);
    bool result = (compare_string(st->s, "A") >= 0 && compare_string(st->s, "Z") <= 0) ||
                  string_in_list(st->s, (char *[]) {"Ç", "Ö", "Ğ", "Ü", "Ş", "İ"}, 6);
    free_string_ptr(st);
    return result;
}

bool is_code_fsm(Fsm_morphological_analyzer_ptr fsm_morphological_analyzer, char *surfaceForm) {
    if (surfaceForm == NULL) {
        return false;
    }
    return pattern_matches(fsm_morphological_analyzer, ".*[0-9].*", surfaceForm) &&
           pattern_matches(fsm_morphological_analyzer, ".*[a-zA-ZçöğüşıÇÖĞÜŞİ].*", surfaceForm);
}

/**
 * The isInteger method compares input surfaceForm with regex \+?\d+ and returns the result.
 * Supports positive integer checks only.
 *
 * @param surfaceForm String to check.
 * @return true if surfaceForm matches with the regex.
 */
bool is_integer(Fsm_morphological_analyzer_ptr fsm_morphological_analyzer, char *surfaceForm) {
    if (!pattern_matches(fsm_morphological_analyzer, "[\\-\\+]?\\d+", surfaceForm)) {
        return false;
    }
    int len = word_size(surfaceForm);
    if (len < 10) {
        return true;
    } else {
        if (len > 10) {
            return false;
        } else {
            return compare_string(surfaceForm, "2147483647") <= 0;
        }
    }
}

/**
 * The isDouble method compares input surfaceForm with regex \+?(\d+)?\.\d* and returns the result.
 *
 * @param surfaceForm String to check.
 * @return true if surfaceForm matches with the regex.
 */
bool is_double(Fsm_morphological_analyzer_ptr fsm_morphological_analyzer, char *surfaceForm) {
    return pattern_matches(fsm_morphological_analyzer, "[\\+\\-]?\\d*\\.\\d*", surfaceForm);
}

/**
 * The isNumber method compares input surfaceForm with the array of written numbers and returns the result.
 *
 * @param surfaceForm String to check.
 * @return true if surfaceForm matches with the regex.
 */
bool is_number_fsm(Fsm_morphological_analyzer_ptr fsm_morphological_analyzer, char *surfaceForm) {
    bool found;
    int count = 0;
    char *numbers[] = {"bir", "iki", "üç", "dört", "beş", "altı", "yedi", "sekiz", "dokuz",
                       "on", "yirmi", "otuz", "kırk", "elli", "altmış", "yetmiş", "seksen", "doksan",
                       "yüz", "bin", "milyon", "milyar", "trilyon", "katrilyon"};
    char *word = str_copy(word, surfaceForm);
    while (word != NULL) {
        found = false;
        for (int i = 0; i < 24; i++) {
            if (starts_with(word, numbers[i])) {
                found = true;
                count++;
                String_ptr st = substring2(word, word_size(numbers[i]));
                free_(word);
                word = str_copy(word, st->s);
                free_string_ptr(st);
                break;
            }
        }
        if (!found) {
            break;
        }
    }
    if (word != NULL) {
        free_(word);
        return false;
    }
    return word == NULL && count > 1;
}

bool is_percent_fsm(Fsm_morphological_analyzer_ptr fsm_morphological_analyzer, char *surface_form) {
    return pattern_matches(fsm_morphological_analyzer, "(%(\\d\\d|\\d))", surface_form) ||
           pattern_matches(fsm_morphological_analyzer, "(%(\\d\\d|\\d)\\.\\d+)", surface_form);
}

bool is_time_fsm(Fsm_morphological_analyzer_ptr fsm_morphological_analyzer, char *surface_form) {
    return pattern_matches(fsm_morphological_analyzer, "((\\d\\d|\\d):(\\d\\d|\\d):(\\d\\d|\\d))", surface_form) ||
           pattern_matches(fsm_morphological_analyzer, "((\\d\\d|\\d):(\\d\\d|\\d))", surface_form);
}

bool is_range_fsm(Fsm_morphological_analyzer_ptr fsm_morphological_analyzer, char *surface_form) {
    return pattern_matches(fsm_morphological_analyzer, "\\d+\\-\\d+", surface_form) ||
           pattern_matches(fsm_morphological_analyzer, "((\\d\\d|\\d):(\\d\\d|\\d)-(\\d\\d|\\d):(\\d\\d|\\d))",
                           surface_form) ||
           pattern_matches(fsm_morphological_analyzer, "((\\d\\d|\\d)\\.(\\d\\d|\\d)-(\\d\\d|\\d)\\.(\\d\\d|\\d))",
                           surface_form);
}

bool is_date_fsm(Fsm_morphological_analyzer_ptr fsm_morphological_analyzer, char *surface_form) {
    return pattern_matches(fsm_morphological_analyzer, "((\\d\\d|\\d)/(\\d\\d|\\d)/\\d+)", surface_form) ||
           pattern_matches(fsm_morphological_analyzer, "((\\d\\d|\\d)\\.(\\d\\d|\\d)\\.\\d+)", surface_form);
}

/**
 * Replaces previous lemma in the sentence with the new lemma. Both lemma can contain multiple words.
 * @param original Original sentence to be replaced with.
 * @param previous_word Root word in the original sentence
 * @param new_word New word to be replaced.
 * @return Newly generated sentence by replacing the previous word in the original sentence with the new word.
 */
Sentence_ptr replace_word_fsm(Fsm_morphological_analyzer_ptr fsm_morphological_analyzer,
                              Sentence_ptr original,
                              char *previous_word,
                              char *new_word) {
    int i;
    Array_list_ptr previous_word_splitted = create_array_list(), new_word_splitted = create_array_list();
    Sentence_ptr result = create_sentence();
    char *replaced_word, *last_word, *new_root_word;
    bool previous_word_multiple = str_contains(previous_word, " ");
    bool new_word_multiple = str_contains(new_word, " ");
    if (previous_word_multiple) {
        previous_word_splitted = str_split(previous_word, ' ');
        last_word = array_list_get(previous_word_splitted, previous_word_splitted->size - 1);
    } else {
        last_word = previous_word;
    }
    if (new_word_multiple) {
        new_word_splitted = str_split(new_word, ' ');
        new_root_word = array_list_get(new_word_splitted, new_word_splitted->size - 1);
    } else {
        new_root_word = new_word;
    }
    Txt_word_ptr new_root_txt_word = get_word_txt(fsm_morphological_analyzer->dictionary, new_root_word);
    Fsm_parse_list_ptr *parse_list = morphological_analysis4(fsm_morphological_analyzer, original);
    for (i = 0; i < original->words->size; i++) {
        bool replaced = false;
        for (int j = 0; j < parse_list[i]->fsm_parses->size; j++) {
            Fsm_parse_ptr fsm_parse = get_fsm_parse(parse_list[i], j);
            if (strcmp(fsm_parse->root->name, last_word) == 0 && new_root_txt_word != NULL) {
                replaced = true;
                replaced_word = replace_root_word(fsm_parse, new_root_txt_word);
            }
        }
        if (replaced && replaced_word != NULL) {
            if (previous_word_multiple) {
                for (int k = 0; k < i - previous_word_splitted->size + 1; k++) {
                    sentence_add_word(result, array_list_get(original->words, k));
                }
            }
            if (new_word_multiple) {
                for (int k = 0; k < new_word_splitted->size - 1; k++) {
                    char *word = array_list_get(new_word_splitted, k);
                    if (result->words->size == 0) {
                        String_ptr st = char_at(word, 0);
                        char *ch = to_uppercase(st->s);
                        String_ptr st2 = create_string2(ch);
                        String_ptr st3 = substring2(word, 1);
                        string_append_s(st2, st3);
                        sentence_add_word_copy(result, st2->s);
                        free_string_ptr(st);
                        free_string_ptr(st2);
                        free_string_ptr(st3);
                        free_(ch);
                    } else {
                        sentence_add_word_copy(result, word);
                    }
                }
            }
            if (result->words->size == 0) {
                String_ptr st = char_at(replaced_word, 0);
                char *ch = to_uppercase(st->s);
                String_ptr st2 = create_string2(ch);
                String_ptr st3 = substring2(replaced_word, 1);
                string_append_s(st2, st3);
                replaced_word = str_copy(replaced_word, st2->s);
                free_string_ptr(st);
                free_string_ptr(st2);
                free_string_ptr(st3);
                free_(ch);
            }
            sentence_add_word_copy(result, replaced_word);
            if (previous_word_multiple) {
                i++;
                break;
            }
        } else {
            if (!previous_word_multiple) {
                sentence_add_word(result, array_list_get(original->words, i));
            }
        }
    }
    if (previous_word_multiple) {
        for (; i < original->words->size; i++) {
            sentence_add_word(result, array_list_get(original->words, i));
        }
    }
    free_array_list(previous_word_splitted, free_);
    free_array_list(new_word_splitted, free_);
    return result;
}

/**
 * The analysisExists method checks several cases. If the given surface_form is a punctuation or double then it
 * returns true. If it is not a root word, then it initializes the parse list and returns the parseExists method with
 * this newly initialized list and surface_form.
 *
 * @param root_word    TxtWord root.
 * @param surface_form String input.
 * @param is_proper    boolean variable indicates a word is proper or not.
 * @return true if surface_form is punctuation or double, otherwise returns parseExist method with given surface_form.
 */
bool analysis_exists(Fsm_morphological_analyzer_ptr fsm_morphological_analyzer,
                     Txt_word_ptr root_word,
                     char *surface_form,
                     bool is_proper) {
    Array_list_ptr initial_fsm_parse;
    if (is_punctuation(surface_form)) {
        return true;
    }
    if (is_double(fsm_morphological_analyzer, surface_form)) {
        return true;
    }
    if (root_word != NULL) {
        initial_fsm_parse = create_array_list();
        initialize_parse_list_from_root(fsm_morphological_analyzer, initial_fsm_parse, root_word, is_proper);
    } else {
        initial_fsm_parse = initialize_parse_list_from_surface_form(fsm_morphological_analyzer, surface_form,
                                                                    is_proper);
    }
    bool result = parse_exists(fsm_morphological_analyzer, initial_fsm_parse, surface_form);
    free_array_list(initial_fsm_parse, (void (*)(void *)) free_fsm_parse);
    return result;
}

/**
 * The analysis method is used by the morphologicalAnalysis method. It gets String surface_form as an input and checks
 * its type such as punctuation, number or compares with the regex for date, fraction, percent, time, range, hashtag,
 * and mail or checks its variable type as integer or double. After finding the right case for given surface_form, it calls
 * constructInflectionalGroups method which creates sub-word units.
 *
 * @param surface_form String to analyse.
 * @param is_proper    is used to indicate the proper words.
 * @return ArrayList type initialFsmParse which holds the analyses.
 */
Array_list_ptr analysis(Fsm_morphological_analyzer_ptr fsm_morphological_analyzer, char *surface_form, bool is_proper) {
    Array_list_ptr initial_fsm_parse = create_array_list();
    Fsm_parse_ptr fsm_parse;
    if (is_punctuation(surface_form) && strcmp(surface_form, "%") != 0) {
        fsm_parse = create_fsm_parse5(surface_form, create_fsm_state(("Punctuation"), true, true));
        construct_inflectional_groups(fsm_parse);
        array_list_add(initial_fsm_parse, fsm_parse);
        return initial_fsm_parse;
    }
    if (is_number_fsm(fsm_morphological_analyzer, surface_form)) {
        fsm_parse = create_fsm_parse5(surface_form, create_fsm_state(("CardinalRoot"), true, true));
        construct_inflectional_groups(fsm_parse);
        array_list_add(initial_fsm_parse, fsm_parse);
        return initial_fsm_parse;
    }
    if (pattern_matches(fsm_morphological_analyzer, "\\d+/\\d+", surface_form)) {
        fsm_parse = create_fsm_parse5(surface_form, create_fsm_state(("FractionRoot"), true, true));
        construct_inflectional_groups(fsm_parse);
        array_list_add(initial_fsm_parse, fsm_parse);
        fsm_parse = create_fsm_parse5(surface_form, create_fsm_state(("DateRoot"), true, true));
        construct_inflectional_groups(fsm_parse);
        array_list_add(initial_fsm_parse, fsm_parse);
        return initial_fsm_parse;
    }
    if (is_date_fsm(fsm_morphological_analyzer, surface_form)) {
        fsm_parse = create_fsm_parse5(surface_form, create_fsm_state(("DateRoot"), true, true));
        construct_inflectional_groups(fsm_parse);
        array_list_add(initial_fsm_parse, fsm_parse);
        return initial_fsm_parse;
    }
    if (pattern_matches(fsm_morphological_analyzer, "\\d+\\\\/\\d+", surface_form)) {
        fsm_parse = create_fsm_parse5(surface_form, create_fsm_state(("FractionRoot"), true, true));
        construct_inflectional_groups(fsm_parse);
        array_list_add(initial_fsm_parse, fsm_parse);
        return initial_fsm_parse;
    }
    if (strcmp(surface_form, "%") == 0 || is_percent_fsm(fsm_morphological_analyzer, surface_form)) {
        fsm_parse = create_fsm_parse5(surface_form, create_fsm_state(("PercentRoot"), true, true));
        construct_inflectional_groups(fsm_parse);
        array_list_add(initial_fsm_parse, fsm_parse);
        return initial_fsm_parse;
    }
    if (is_time_fsm(fsm_morphological_analyzer, surface_form)) {
        fsm_parse = create_fsm_parse5(surface_form, create_fsm_state(("TimeRoot"), true, true));
        construct_inflectional_groups(fsm_parse);
        array_list_add(initial_fsm_parse, fsm_parse);
        return initial_fsm_parse;
    }
    if (is_range_fsm(fsm_morphological_analyzer, surface_form)) {
        fsm_parse = create_fsm_parse5(surface_form, create_fsm_state(("RangeRoot"), true, true));
        construct_inflectional_groups(fsm_parse);
        array_list_add(initial_fsm_parse, fsm_parse);
        return initial_fsm_parse;
    }
    if (starts_with(surface_form, "#")) {
        fsm_parse = create_fsm_parse5(surface_form, create_fsm_state(("Hashtag"), true, true));
        construct_inflectional_groups(fsm_parse);
        array_list_add(initial_fsm_parse, fsm_parse);
        return initial_fsm_parse;
    }
    if (str_contains_c(surface_form, '@')) {
        fsm_parse = create_fsm_parse5(surface_form, create_fsm_state(("Email"), true, true));
        construct_inflectional_groups(fsm_parse);
        array_list_add(initial_fsm_parse, fsm_parse);
        return initial_fsm_parse;
    }
    String_ptr st1 = last_char(surface_form);
    String_ptr st2 = substring_except_last_char(surface_form);
    if (string_equals2(st1, ".") && is_integer(fsm_morphological_analyzer, st2->s)) {
        fsm_parse = create_fsm_parse3(atoi(st2->s),
                                      get_state(fsm_morphological_analyzer->finite_state_machine, "OrdinalRoot"));
        free_string_ptr(st1);
        free_string_ptr(st2);
        construct_inflectional_groups(fsm_parse);
        array_list_add(initial_fsm_parse, fsm_parse);
        return initial_fsm_parse;
    }
    free_string_ptr(st1);
    free_string_ptr(st2);
    if (is_integer(fsm_morphological_analyzer, surface_form)) {
        fsm_parse = create_fsm_parse3(atoi(surface_form),
                                      get_state(fsm_morphological_analyzer->finite_state_machine, "CardinalRoot"));
        construct_inflectional_groups(fsm_parse);
        array_list_add(initial_fsm_parse, fsm_parse);
        return initial_fsm_parse;
    }
    if (is_double(fsm_morphological_analyzer, surface_form)) {
        fsm_parse = create_fsm_parse4(atof(surface_form),
                                      get_state(fsm_morphological_analyzer->finite_state_machine, "RealRoot"));
        construct_inflectional_groups(fsm_parse);
        array_list_add(initial_fsm_parse, fsm_parse);
        return initial_fsm_parse;
    }
    free_array_list(initial_fsm_parse, NULL);
    initial_fsm_parse = initialize_parse_list_from_surface_form(fsm_morphological_analyzer, surface_form, is_proper);
    Array_list_ptr result_fsm_parse = parse_word2(fsm_morphological_analyzer, initial_fsm_parse, surface_form);
    free_array_list(initial_fsm_parse, NULL);
    return result_fsm_parse;
}

Txt_word_ptr root_of_possibly_new_word(Fsm_morphological_analyzer_ptr fsm_morphological_analyzer, char *surfaceForm) {
    char* reverse = reverse_string(surfaceForm);
    Hash_set_ptr words = get_words_with_prefix(fsm_morphological_analyzer->suffix_trie, reverse);
    free_(reverse);
    int max_length = 0;
    char *longest_word = NULL;
    Array_list_ptr list = hash_set_key_list(words);
    for (int i = 0; i < list->size; i++) {
        Txt_word_ptr word = array_list_get(list, i);
        if (word_size(word->name) > max_length) {
            String_ptr st = substring(surfaceForm, 0, word_size(surfaceForm) - word_size(word->name));
            free_(longest_word);
            longest_word = str_copy(longest_word, st->s);
            free_string_ptr(st);
            max_length = word_size(word->name);
        }
    }
    free_hash_set(words, (void (*)(void *)) free_txt_word);
    free_array_list(list, NULL);
    Txt_word_ptr new_word;
    if (max_length != 0) {
        if (ends_with(longest_word, "ğ")) {
            String_ptr st = substring(longest_word, 0, word_size(longest_word) - 1);
            string_append(st, "k");
            free_(longest_word);
            longest_word = str_copy(longest_word, st->s);
            free_string_ptr(st);
            new_word = create_txt_word2(longest_word, "CL_ISIM");
            add_flag(new_word, "IS_SD");
        } else {
            new_word = create_txt_word2(longest_word, "CL_ISIM");
            add_flag(new_word, "CL_FIIL");
        }
        add_word_to_trie(fsm_morphological_analyzer->dictionary_trie, longest_word, new_word);
        return new_word;
    }
    return NULL;
}

/**
 * The robustMorphologicalAnalysis is used to analyse surfaceForm String. First it gets the currentParse of the surfaceForm
 * then, if the size of the currentParse is 0, and given surfaceForm is a proper noun, it adds the surfaceForm
 * whose state name is ProperRoot to an ArrayList, of it is not a proper noon, it adds the surfaceForm
 * whose state name is NominalRoot to the ArrayList.
 *
 * @param surfaceForm String to analyse.
 * @return FsmParseList type currentParse which holds morphological analysis of the surfaceForm.
 */
Fsm_parse_list_ptr
robust_morphological_analysis(Fsm_morphological_analyzer_ptr fsm_morphological_analyzer, char *surfaceForm) {
    Array_list_ptr fsm_parse;
    Fsm_parse_list_ptr current_parse;
    if (surfaceForm == NULL) {
        return create_fsm_parse_list2();
    }
    current_parse = morphological_analysis(fsm_morphological_analyzer, surfaceForm);
    if (current_parse->fsm_parses->size == 0) {
        free_fsm_parse_list(current_parse);
        fsm_parse = create_array_list();
        if (is_proper_noun_fsm(surfaceForm)) {
            array_list_add(fsm_parse, create_fsm_parse5(surfaceForm,
                                                        get_state(fsm_morphological_analyzer->finite_state_machine,
                                                                  "ProperRoot")));
        } else {
            if (is_code_fsm(fsm_morphological_analyzer, surfaceForm)) {
                array_list_add(fsm_parse, create_fsm_parse5(surfaceForm,
                                                            get_state(fsm_morphological_analyzer->finite_state_machine,
                                                                      "CodeRoot")));
            } else {
                Txt_word_ptr newRoot = root_of_possibly_new_word(fsm_morphological_analyzer, surfaceForm);
                if (newRoot != NULL) {
                    array_list_add(fsm_parse, create_fsm_parse6(newRoot, get_state(
                            fsm_morphological_analyzer->finite_state_machine, "VerbalRoot")));
                    array_list_add(fsm_parse, create_fsm_parse6(newRoot, get_state(
                            fsm_morphological_analyzer->finite_state_machine, "NominalRoot")));
                } else {
                    array_list_add(fsm_parse, create_fsm_parse5(surfaceForm, get_state(
                            fsm_morphological_analyzer->finite_state_machine, "NominalRoot")));
                }
            }
        }
        Array_list_ptr result = parse_word2(fsm_morphological_analyzer, fsm_parse, surfaceForm);
        free_array_list(fsm_parse, NULL);
        return create_fsm_parse_list(result);
    } else {
        return current_parse;
    }
}

/**
 * The morphologicalAnalysis is used for debug purposes.
 *
 * @param sentence  to get word from.
 * @return FsmParseList type result.
 */
Fsm_parse_list_ptr *morphological_analysis4(Fsm_morphological_analyzer_ptr fsm_morphological_analyzer, Sentence_ptr sentence) {
    Fsm_parse_list_ptr word_fsm_parse_list;
    Fsm_parse_list_ptr *result = malloc_(sentence_word_count(sentence) * sizeof(Fsm_parse_list_ptr), "morphological_analysis4");
    for (int i = 0; i < sentence_word_count(sentence); i++) {
        char *original_form = sentence_get_word(sentence, i);
        char *spell_corrected_form = get_correct_form(fsm_morphological_analyzer->dictionary, original_form);
        if (spell_corrected_form == NULL) {
            spell_corrected_form = original_form;
        }
        word_fsm_parse_list = morphological_analysis(fsm_morphological_analyzer, spell_corrected_form);
        result[i] = word_fsm_parse_list;
    }
    return result;
}

/**
 * The robustMorphologicalAnalysis method takes just one argument as an input. It gets the name of the words from
 * input sentence then calls robustMorphologicalAnalysis with surfaceForm.
 *
 * @param sentence Sentence type input used to get surfaceForm.
 * @return FsmParseList array which holds the result of the analysis.
 */
Fsm_parse_list_ptr *robust_morphological_analysis2(Fsm_morphological_analyzer_ptr fsm_morphological_analyzer, Sentence_ptr sentence) {
    Fsm_parse_list_ptr word_fsm_parse_list;
    Fsm_parse_list_ptr *result = malloc_(sentence_word_count(sentence) * sizeof(Fsm_parse_list_ptr), "robust_morphological_analysis2");
    for (int i = 0; i < sentence_word_count(sentence); i++) {
        char *original_form = sentence_get_word(sentence, i);
        char *spell_corrected_form = get_correct_form(fsm_morphological_analyzer->dictionary, original_form);
        if (spell_corrected_form == NULL) {
            spell_corrected_form = original_form;
        }
        word_fsm_parse_list = robust_morphological_analysis(fsm_morphological_analyzer, spell_corrected_form);
        result[i] = word_fsm_parse_list;
    }
    return result;
}

/**
 * The morphologicalAnalysisExists method calls analysisExists to check the existence of the analysis with given
 * root and surfaceForm.
 *
 * @param surfaceForm String to check.
 * @param rootWord    TxtWord input root.
 * @return true an analysis exists, otherwise return false.
 */
bool morphological_analysis_exists(Fsm_morphological_analyzer_ptr fsm_morphological_analyzer, Txt_word_ptr rootWord,
                                   char *surfaceForm) {
    return analysis_exists(fsm_morphological_analyzer, rootWord, to_lowercase(surfaceForm), true);
}

/**
 * The morphologicalAnalysis method is used to analyse a FsmParseList by comparing with the regex.
 * It creates an ArrayList fsmParse to hold the result of the analysis method. For each surfaceForm input,
 * it gets a substring and considers it as a possibleRoot. Then compares with the regex.
 * <p>
 * If the surfaceForm input string matches with Turkish chars like Ç, Ş, İ, Ü, Ö, it adds the surfaceForm to Trie with IS_OA tag.
 * If the possibleRoot contains /, then it is added to the Trie with IS_KESIR tag.
 * If the possibleRoot contains \d\d|\d)/(\d\d|\d)/\d+, then it is added to the Trie with IS_DATE tag.
 * If the possibleRoot contains \\d\d|\d, then it is added to the Trie with IS_PERCENT tag.
 * If the possibleRoot contains \d\d|\d):(\d\d|\d):(\d\d|\d), then it is added to the Trie with IS_ZAMAN tag.
 * If the possibleRoot contains \d+-\d+, then it is added to the Trie with IS_RANGE tag.
 * If the possibleRoot is an Integer, then it is added to the Trie with IS_SAYI tag.
 * If the possibleRoot is a Double, then it is added to the Trie with IS_REELSAYI tag.
 *
 * @param surfaceForm String to analyse.
 * @return fsmParseList which holds the analysis.
 */
Fsm_parse_list_ptr
morphological_analysis(Fsm_morphological_analyzer_ptr fsm_morphological_analyzer, char *surfaceForm) {
    Fsm_parse_list_ptr fsmParseList;
    char* lowerCased = to_lowercase(surfaceForm);
    if (fsm_morphological_analyzer->parsed_surface_forms != NULL &&
        fsm_morphological_analyzer->parsed_surface_forms->count > 0 &&
        hash_map_contains(fsm_morphological_analyzer->parsed_surface_forms, lowerCased) &&
        !is_integer(fsm_morphological_analyzer, surfaceForm) && !is_double(fsm_morphological_analyzer, surfaceForm) &&
        !is_percent_fsm(fsm_morphological_analyzer, surfaceForm) &&
        !is_time_fsm(fsm_morphological_analyzer, surfaceForm) &&
        !is_range_fsm(fsm_morphological_analyzer, surfaceForm) &&
        !is_date_fsm(fsm_morphological_analyzer, surfaceForm)) {
        Array_list_ptr parses = create_array_list();
        array_list_add(parses, create_fsm_parse(create_txt_word(hash_map_get(fsm_morphological_analyzer->parsed_surface_forms, lowerCased))));
        free_(lowerCased);
        return create_fsm_parse_list(parses);
    }
    if (fsm_morphological_analyzer->cache->cache_size > 0 &&
        lru_cache_contains(fsm_morphological_analyzer->cache, surfaceForm)) {
        free_(lowerCased);
        return clone_fsm_parse_list(lru_cache_get(fsm_morphological_analyzer->cache, surfaceForm));
    }
    if (pattern_matches(fsm_morphological_analyzer, "([A-Z]|Ç|Ş|İ|Ü|Ö)\\.", surfaceForm)) {
        add_word_to_trie(fsm_morphological_analyzer->dictionary_trie, lowerCased,
                         create_txt_word2(lowerCased, "IS_OA"));
    }
    Array_list_ptr defaultFsmParse = analysis(fsm_morphological_analyzer, lowerCased, is_proper_noun_fsm(surfaceForm));
    if (defaultFsmParse->size > 0) {
        fsmParseList = create_fsm_parse_list(defaultFsmParse);
        if (fsm_morphological_analyzer->cache->cache_size > 0) {
            lru_cache_add(fsm_morphological_analyzer->cache, surfaceForm, clone_fsm_parse_list(fsmParseList));
        }
        free_(lowerCased);
        return fsmParseList;
    } else {
        free_array_list(defaultFsmParse, (void (*)(void *)) free_fsm_parse);
    }
    Array_list_ptr fsmParse = NULL;
    if (str_contains(surfaceForm, "'")) {
        String_ptr st = substring(surfaceForm, 0, str_find_c(surfaceForm, "'"));
        char *possibleRoot = str_copy(possibleRoot, st->s);
        free_string_ptr(st);
        if (possibleRoot != NULL) {
            if (str_contains_c(possibleRoot, '/') || str_contains(possibleRoot, "\\/")) {
                add_word_to_trie(fsm_morphological_analyzer->dictionary_trie, possibleRoot,
                                 create_txt_word2(possibleRoot, "IS_KESIR"));
                fsmParse = analysis(fsm_morphological_analyzer, lowerCased,
                                    is_proper_noun_fsm(surfaceForm));
            } else {
                if (is_date_fsm(fsm_morphological_analyzer, possibleRoot)) {
                    add_word_to_trie(fsm_morphological_analyzer->dictionary_trie, possibleRoot,
                                     create_txt_word2(possibleRoot, "IS_DATE"));
                    fsmParse = analysis(fsm_morphological_analyzer, lowerCased,
                                        is_proper_noun_fsm(surfaceForm));
                } else {
                    if (pattern_matches(fsm_morphological_analyzer, "\\d+/\\d+", possibleRoot)) {
                        add_word_to_trie(fsm_morphological_analyzer->dictionary_trie, possibleRoot,
                                         create_txt_word2(possibleRoot, "IS_KESIR"));
                        fsmParse = analysis(fsm_morphological_analyzer, lowerCased,
                                            is_proper_noun_fsm(surfaceForm));
                    } else {
                        if (is_percent_fsm(fsm_morphological_analyzer, possibleRoot)) {
                            add_word_to_trie(fsm_morphological_analyzer->dictionary_trie, possibleRoot,
                                             create_txt_word2(possibleRoot, "IS_PERCENT"));
                            fsmParse = analysis(fsm_morphological_analyzer, lowerCased,
                                                is_proper_noun_fsm(surfaceForm));
                        } else {
                            if (is_time_fsm(fsm_morphological_analyzer, possibleRoot)) {
                                add_word_to_trie(fsm_morphological_analyzer->dictionary_trie, possibleRoot,
                                                 create_txt_word2(possibleRoot, "IS_ZAMAN"));
                                fsmParse = analysis(fsm_morphological_analyzer, lowerCased,
                                                    is_proper_noun_fsm(surfaceForm));
                            } else {
                                if (is_range_fsm(fsm_morphological_analyzer, possibleRoot)) {
                                    add_word_to_trie(fsm_morphological_analyzer->dictionary_trie, possibleRoot,
                                                     create_txt_word2(possibleRoot, "IS_RANGE"));
                                    fsmParse = analysis(fsm_morphological_analyzer, lowerCased,
                                                        is_proper_noun_fsm(surfaceForm));
                                } else {
                                    if (is_integer(fsm_morphological_analyzer, possibleRoot)) {
                                        add_word_to_trie(fsm_morphological_analyzer->dictionary_trie, possibleRoot,
                                                         create_txt_word2(possibleRoot, "IS_SAYI"));
                                        fsmParse = analysis(fsm_morphological_analyzer, lowerCased, is_proper_noun_fsm(surfaceForm));
                                    } else {
                                        if (is_double(fsm_morphological_analyzer, possibleRoot)) {
                                            add_word_to_trie(fsm_morphological_analyzer->dictionary_trie, possibleRoot,
                                                             create_txt_word2(possibleRoot, "IS_REELSAYI"));
                                            fsmParse = analysis(fsm_morphological_analyzer, lowerCased,
                                                                is_proper_noun_fsm(surfaceForm));
                                        } else {
                                            if (is_capital(possibleRoot)) {
                                                Txt_word_ptr newWord = NULL;
                                                char *lowerCase = to_lowercase(possibleRoot);
                                                if (get_word_txt(fsm_morphological_analyzer->dictionary, lowerCase) !=
                                                    NULL) {
                                                    add_flag(get_word_txt(fsm_morphological_analyzer->dictionary,
                                                                          lowerCase), "IS_OA");
                                                } else {
                                                    newWord = create_txt_word2(lowerCase, "IS_OA");
                                                    add_word_to_trie(fsm_morphological_analyzer->dictionary_trie,
                                                                     lowerCase, newWord);
                                                }
                                                free_(lowerCase);
                                                lowerCase = to_lowercase(surfaceForm);
                                                fsmParse = analysis(fsm_morphological_analyzer,
                                                                    lowerCase,
                                                                    is_proper_noun_fsm(surfaceForm));
                                                if (fsmParse == NULL && newWord != NULL) {
                                                    add_flag(newWord, "IS_KIS");
                                                    fsmParse = analysis(fsm_morphological_analyzer,
                                                                        lowerCase,
                                                                        is_proper_noun_fsm(surfaceForm));
                                                }
                                                free_(lowerCase);
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        free_(possibleRoot);
    }
    free_(lowerCased);
    if (fsmParse == NULL){
        fsmParse = create_array_list();
    }
    fsmParseList = create_fsm_parse_list(fsmParse);
    if (fsm_morphological_analyzer->cache->cache_size > 0 && fsmParseList->fsm_parses->size > 0) {
        lru_cache_add(fsm_morphological_analyzer->cache, surfaceForm, clone_fsm_parse_list(fsmParseList));
    }
    return fsmParseList;
}

Fsm_morphological_analyzer_ptr create_fsm_morphological_analyzer3() {
    return create_fsm_morphological_analyzer("turkish_finite_state_machine.xml", create_txt_dictionary(), 10000);
}
