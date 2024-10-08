//
// Created by Olcay Taner YILDIZ on 19.10.2023.
//

#include <Dictionary/Word.h>
#include <string.h>
#include <Language/TurkishLanguage.h>
#include <Memory/Memory.h>
#include "Transition.h"
#include "MorphotacticEngine.h"

/**
 * A constructor of Transition class which takes  a Fsm_State, and two Strings as input. Then it
 * initializes toState, with and withName variables with given inputs.
 *
 * @param toState  Fsm_State input.
 * @param with     String input.
 * @param withName String input.
 */
Transition_ptr create_transition(Fsm_State_ptr to_state,
                                 char *with,
                                 char *with_name) {
    Transition_ptr result = malloc_(sizeof(Transition), "create_transition");
    result->to_state = to_state;
    result->with = str_copy(result->with, with);
    result->with_name = str_copy(result->with_name, with_name);
    result->to_pos = NULL;
    return result;
}

/**
 * Another constructor of Transition class which takes  a Fsm_State, and three Strings as input. Then it
 * initializes toState, with, withName and toPos variables with given inputs.
 *
 * @param toState  Fsm_State input.
 * @param with     String input.
 * @param withName String input.
 * @param toPos    String input.
 */
Transition_ptr create_transition2(Fsm_State_ptr to_state,
                                  char *with,
                                  char *with_name,
                                  char *to_pos) {
    Transition_ptr result = malloc_(sizeof(Transition), "create_transition2");
    result->to_state = to_state;
    result->with = str_copy(result->with, with);
    result->with_name = str_copy(result->with_name, with_name);
    result->to_pos = str_copy(result->to_pos, to_pos);
    return result;
}

/**
 * Another constructor of Transition class which only takes a Strings as an input. Then it
 * initializes toState, withName and toPos variables as null and with variable with the given input.
 *
 * @param with String input.
 */
Transition_ptr create_transition3(char *with) {
    Transition_ptr result = malloc_(sizeof(Transition), "create_transition3");
    result->to_state = NULL;
    result->with = str_copy(result->with, with);
    result->with_name = NULL;
    result->to_pos = NULL;
    return result;
}

/**
 * Frees memory allocated for transition object. Deallocates strings.
 * @param transition Transition to be deallocated.
 */
void free_transition(Transition_ptr transition) {
    free_(transition->with);
    free_(transition->with_name);
    free_(transition->to_pos);
    free_(transition);
}

/**
 * The transitionPossible method takes two String as inputs; current_surface_form and real_surface_form. If the
 * length of the given current_surface_form is greater than the given real_surface_form, it directly returns true. If not,
 * it takes a substring from given real_surface_form with the size of current_surface_form. Then checks for the characters of
 * with variable.
 * <p>
 * If the character of with that makes transition is C, it returns true if the substring contains c or ç.
 * If the character of with that makes transition is D, it returns true if the substring contains d or t.
 * If the character of with that makes transition is A, it returns true if the substring contains a or e.
 * If the character of with that makes transition is K, it returns true if the substring contains k, g or ğ.
 * If the character of with that makes transition is other than the ones above, it returns true if the substring
 * contains the same character as with.
 *
 * @param current_surface_form String input.
 * @param real_surface_form    String input.
 * @return true when the transition is possible according to Turkish grammar, false otherwise.
 */
bool transition_possible1(Transition_ptr transition, char *current_surface_form, char *real_surface_form) {
    if (word_size(current_surface_form) == 0 || word_size(current_surface_form) >= word_size(real_surface_form)) {
        return true;
    }
    bool result = true;
    String_ptr search = substring(real_surface_form, word_size(current_surface_form), word_size(real_surface_form));
    char search_string[strlen(search->s) + 1];
    strcpy(search_string, search->s);
    free_string_ptr(search);
    Array_list_ptr withChars = all_characters(transition->with);
    for (int i = 0; i < word_size(transition->with); i++) {
        String_ptr st = array_list_get(withChars, i);
        char ch[strlen(st->s) + 1];
        strcpy(ch, st->s);
        if (strcmp(ch, "C") == 0) {
            result = str_contains(search_string, "c") || str_contains(search_string, "ç");
            break;
        } else {
            if (strcmp(ch, "D") == 0) {
                result = str_contains(search_string, "d") || str_contains(search_string, "t");
                break;
            } else {
                if (string_in_list(ch, (char*[]) {"c", "e", "r", "p", "l", "b", "g", "o", "m", "v", "i", "ü", "z"}, 13)) {
                    result = str_contains(search_string, ch);
                    break;
                } else {
                    if (strcmp(ch, "A") == 0) {
                        result = str_contains(search_string, "a") || str_contains(search_string, "e");
                        break;
                    } else {
                        if (strcmp(ch, "k") == 0) {
                            result = str_contains(search_string, "k") || str_contains(search_string, "g") ||
                                   str_contains(search_string, "ğ");
                            break;
                        }
                    }
                }
            }
        }
    }
    free_array_list(withChars, (void (*)(void *)) free_string_ptr);
    return result;
}

/**
 * The transitionPossible method takes a current parse as an input. It then checks some special  cases.
 *
 * @param currentFsmParse Parse to be checked
 * @return true if transition is possible false otherwise
 */
bool transition_possible2(Transition_ptr transition, Fsm_parse_ptr currentFsmParse) {
    if (strcmp(transition->with, "Ar") == 0 && ends_with(currentFsmParse->form, "l") && strcmp(currentFsmParse->root->name, currentFsmParse->form) != 0) {
        return false;
    }
    if (currentFsmParse->verb_agreement != NULL && currentFsmParse->possessive_agreement != NULL && transition->with_name != NULL) {
        if (strcmp(currentFsmParse->verb_agreement, "A3PL") == 0 && strcmp(transition->with_name, "^DB+VERB+ZERO+PRES+A1SG") == 0) {
            return false;
        }
        if (strcmp(currentFsmParse->verb_agreement, "A3SG") == 0 && (strcmp(currentFsmParse->possessive_agreement, "P1SG") == 0 || strcmp(currentFsmParse->possessive_agreement, "P2SG") == 0) && strcmp(transition->with_name, "^DB+VERB+ZERO+PRES+A1PL") == 0) {
            return false;
        }
    }
    return true;
}

/**
 * The transitionPossible method takes root and current parse as inputs. It then checks some special cases.
 *
 * @param root Current root word
 * @param fromState From which state we arrived to this state.
 * @return true if transition is possible false otherwise
 */
bool transition_possible3(Transition_ptr transition, Txt_word_ptr root, Fsm_State_ptr from_state) {
    if (is_adjective(root) && ((is_nominal(root) && !is_exceptional(root)) || is_pronoun(root)) &&
        strcmp(transition->to_state->name, "NominalRoot(ADJ)") == 0 && strcmp(transition->with, "0") == 0) {
        return false;
    }
    if (is_adjective(root) && is_nominal(root) && strcmp(transition->with, "^DB+VERB+ZERO+PRES+A3PL") == 0 &&
        strcmp(from_state->name, "AdjectiveRoot") == 0) {
        return false;
    }
    if (is_adjective(root) && is_nominal(root) && strcmp(transition->with, "SH") == 0 &&
        strcmp(from_state->name, "AdjectiveRoot") == 0) {
        return false;
    }
    if (strcmp(transition->with, "ki") == 0) {
        return takes_relative_suffix_ki(root);
    }
    if (strcmp(transition->with, "kü") == 0) {
        return takes_relative_suffix_ku(root);
    }
    if (strcmp(transition->with, "DHr") == 0) {
        if (strcmp(transition->to_state->name, "Adverb") == 0) {
            return true;
        } else {
            return takes_suffix_dir_as_factitive(root);
        }
    }
    if (strcmp(transition->with, "Hr") == 0 && (strcmp(transition->to_state->name, "AdjectiveRoot(VERB)") == 0 ||
                                                (strcmp(transition->to_state->name, "OtherTense") == 0 ||
                                                 strcmp(transition->to_state->name, "OtherTense2") == 0))) {
        return takes_suffix_ir_as_aorist(root);
    }
    return true;
}

/**
 * The withFirstChar method returns the first character of the with variable.
 *
 * @return the first character of the with variable.
 */
char *with_first_char(Transition_ptr transition) {
    if (transition->with == NULL) {
        return "$";
    }
    char *ch = NULL;
    String_ptr st = char_at(transition->with, 0);
    ch = str_copy(ch, st->s);
    free_string_ptr(st);
    if (strcmp(ch, "'") != 0) {
        return ch;
    } else {
        if (word_size(transition->with) == 1) {
            return ch;
        } else {
            st = char_at(transition->with, 1);
            free_(ch);
            ch = str_copy(ch, st->s);
            free_string_ptr(st);
            return ch;
        }
    }
}

/**
 * The startWithVowelorConsonantDrops method checks for some cases. If the first character of with variable is "nsy",
 * and with variable does not equal to one of the Strings; "ylA, ysA, ymHs, yDH, yken", it returns true. If
 * <p>
 * Or, if the first character of with variable is 'A, H: or any other vowels, it returns true.
 *
 * @return true if it starts with vowel or consonant drops, false otherwise.
 */
bool start_with_vowel_or_consonant_drops(Transition_ptr transition) {
    char *tmp = with_first_char(transition);
    char ch[3];
    strcpy(ch, tmp);
    free_(tmp);
    if (is_consonant_drop(ch) &&
        !string_in_list(transition->with, (char *[]) {"ylA", "ysA", "ymHs", "yDH", "yken"}, 5)) {
        return true;
    }
    if (strcmp(ch, "A") == 0 || strcmp(ch, "H") == 0 || is_vowel(ch)) {
        return true;
    }
    return false;
}

/**
 * The softenDuringSuffixation method takes a TxtWord root as an input. It checks two cases; first case returns
 * true if the given root is nominal or adjective and has one of the flags "IS_SD, IS_B_SD, IS_SDD" and with variable
 * equals o one of the Strings "Hm, nDAn, ncA, nDA, yA, yHm, yHz, yH, nH, nA, nHn, H, sH, Hn, HnHz, HmHz".
 * <p>
 * And the second case returns true if the given root is verb and has the "F_SD" flag, also with variable starts with
 * "Hyor" or equals one of the Strings "yHs, yAn, yA, yAcAk, yAsH, yHncA, yHp, yAlH, yArAk, yAdur, yHver, yAgel, yAgor,
 * yAbil, yAyaz, yAkal, yAkoy, yAmA, yHcH, HCH, Hr, Hs, Hn, yHn", yHnHz, Ar, Hl").
 *
 * @param root TxtWord input.
 * @return true if there is softening during suffixation of the given root, false otherwise.
 */
bool soften_during_suffixation(Transition_ptr transition, Txt_word_ptr root) {
    if ((is_nominal(root) || is_adjective(root)) && noun_soften_during_suffixation(root) &&
        string_in_list(transition->with,
                       (char *[]) {"Hm", "nDAn", "ncA", "nDA", "yA",
                                   "yHm", "yHz", "yH", "nH",
                                   "nA", "nHn", "H", "sH",
                                   "Hn", "HnHz", "HmHz"}, 16)) {
        return true;
    }
    if (is_verb(root) && verb_soften_during_suffixation(root) &&
        (starts_with(transition->with, "Hyor") || starts_with(transition->with, "yAcAk") ||
         starts_with(transition->with, "Hr") || starts_with(transition->with, "Ar") || string_in_list(transition->with, (
        char*[]){ "yHs", "yAn", "yA", "yAsH", "yHncA", "yHp", "yAlH", "yArAk",
                   "yAdur", "yHver", "yAgel", "yAgor", "yAbil", "yAyaz", "yAkal", "yAkoy",
                   "yAmA", "yHcH", "HCH", "Hs", "Hn", "yHn", "yHnHz", "Hl"}, 24))) {
        return true;
    }
    return false;
}

/**
 * The makeTransition method takes a TxtWord root and s String stem as inputs. If given root is a verb,
 * it makes transition with given root and stem with the verbal root fsm_state. If given root is not verb, it makes transition
 * with given root and stem and the nominal root fsm_state.
 *
 * @param root TxtWord input.
 * @param stem String input.
 * @return String type output that has the transition.
 */
char *make_transition(Transition_ptr transition, Txt_word_ptr root, char *stem) {
    if (is_verb(root)) {
        return make_transition2(transition, root, stem, create_fsm_state("VerbalRoot", true, false));
    } else {
        return make_transition2(transition, root, stem, create_fsm_state("NominalRoot", true, false));
    }
}

/**
 * The method is main driving method to accomplish the current transition from one state to another depending on
 * the root form of the word, current value of the word form, and the type of the start state. The method
 * (a) returns the original word form if the transition is an epsilon transition, (b) adds 'nunla' if the current
 * stem is 'bu', 'şu' or 'o', (c) returns 'bana' or 'sana' if the current stem is 'ben' or 'sen' respectively.
 * For other cases, the method first modifies current stem and then adds the transition using special metamorpheme
 * resolving methods. These cases are: (d) Converts 'y' of the first character of the transition to 'i' if the
 * current stem is 'ye' or 'de'. (e) Drops the last two characters and adds last character when the transition is
 * ('Hl' or 'Hn') and last 'I' drops during passive suffixation. (f) Adds 'y' character when the word ends with 'su'
 * and the transition does not start with 'y'. (g) Adds the last character again when the root duplicates during
 * suffixation. (h) Drops the last two characters and adds the last character when last 'i' drops during
 * suffixation. (i) Replaces the last character with a soft one when the root soften during suffixation.
 * @param root Root of the current word form
 * @param stem Current word form
 * @param startState The state from which this Fsm morphological analysis search has started.
 * @return The current value of the word form after this transition is completed in the finite state machine.
 */
char *make_transition2(Transition_ptr transition, Txt_word_ptr root, char *stem, Fsm_State_ptr startState) {
    char* tmp3 = str_concat(root->name, "'");
    bool rootWord = strcmp(root->name, stem) == 0 || strcmp(tmp3, stem) == 0;
    free_(tmp3);
    String_ptr formation = create_string2(stem);
    String_ptr formation_to_check;
    bool formation_to_check_allocated = false;
    char* result;
    char except_last[MAX_WORD_LENGTH], except_last_two[MAX_WORD_LENGTH], last[3], last_p[3], with_f[3];
    int i = 0;
    String_ptr tmp = substring_except_last_char(stem);
    strcpy(except_last, tmp->s);
    free_string_ptr(tmp);
    tmp = substring_except_last_two_chars(stem);
    strcpy(except_last_two, tmp->s);
    free_string_ptr(tmp);
    tmp = last_char(stem);
    strcpy(last, tmp->s);
    free_string_ptr(tmp);
    tmp = last_phoneme(stem);
    strcpy(last_p, tmp->s);
    free_string_ptr(tmp);
    char* tmp2 = with_first_char(transition);
    strcpy(with_f, tmp2);
    free_(tmp2);
    if (strcmp(transition->with, "0") == 0) {
        free_string_ptr(formation);
        result = str_copy(result, stem);
        return result;
    }
    if (string_in_list(stem, (char*[]){"bu", "şu", "o"}, 3) && rootWord && strcmp(transition->with, "ylA") == 0) {
        free_string_ptr(formation);
        return str_concat(stem, "nunla");
    }
    if (strcmp(transition->with, "yA") == 0) {
        if (strcmp(stem, "ben") == 0) {
            free_string_ptr(formation);
            result = str_copy(result, "bana");
            return result;
        }
        if (strcmp(stem, "sen") == 0) {
            free_string_ptr(formation);
            result = str_copy(result, "sana");
            return result;
        }
    }
    //---vowelEChangesToIDuringYSuffixation---
    //de->d(i)yor, ye->y(i)yor
    String_ptr with_first = char_at(transition->with, 1);
    String_ptr with_zero = char_at(transition->with, 0);
    if (rootWord && strcmp(with_f, "y") == 0 && vowel_e_changes_to_i_during_y_suffixation(root) && (strcmp(with_first->s, "H") != 0 || strcmp(root->name, "ye") == 0)) {
        free_string_ptr(formation);
        formation = create_string3(except_last, "i");
        formation_to_check = formation;
    } else {
        //---lastIdropsDuringPassiveSuffixation---
        // yoğur->yoğrul, ayır->ayrıl, buyur->buyrul, çağır->çağrıl, çevir->çevril, devir->devril,
        // kavur->kavrul, kayır->kayrıl, kıvır->kıvrıl, savur->savrul, sıyır->sıyrıl, yoğur->yoğrul
        if (rootWord && (strcmp(transition->with, "Hl") == 0 || strcmp(transition->with, "Hn") == 0) &&
                last_i_drops_during_passive_suffixation(root)) {
            free_string_ptr(formation);
            formation = create_string3(except_last_two, last);
            formation_to_check = create_string2(stem);
            formation_to_check_allocated = true;
        } else {
            //---showsSuRegularities---
            //karasu->karasuyu, özsu->özsuyu, ağırsu->ağırsuyu, akarsu->akarsuyu, bengisu->bengisuyu
            if (rootWord && shows_su_regularities(root) && start_with_vowel_or_consonant_drops(transition) && !starts_with(transition->with, "y")) {
                free_string_ptr(formation);
                formation = create_string3(stem, "y");
                i = 1;
                formation_to_check = formation;
            } else {
                if (rootWord && duplicates_during_suffixation(root) && !starts_with(startState->name, "VerbalRoot") && is_consonant_drop(with_zero->s)) {
                    //---duplicatesDuringSuffixation---
                    if (soften_during_suffixation(transition, root)) {
                        //--extra softenDuringSuffixation
                        if (strcmp(last_p, "p") == 0){
                            //tıp->tıbbı
                            free_string_ptr(formation);
                            formation = create_string3(except_last, "bb");
                        } else {
                            if (strcmp(last_p, "t") == 0){
                                //cet->ceddi, met->meddi, ret->reddi, serhat->serhaddi, zıt->zıddı, şet->şeddi
                                free_string_ptr(formation);
                                formation = create_string3(except_last, "dd");
                            }
                        }
                    } else {
                        //cer->cerri, emrihak->emrihakkı, fek->fekki, fen->fenni, had->haddi, hat->hattı,
                        // haz->hazzı, his->hissi
                        free_string_ptr(formation);
                        formation = create_string3(stem, last);
                    }
                    formation_to_check = formation;
                } else {
                    if (rootWord && last_i_drops_during_suffixation(root) && !starts_with(startState->name, "VerbalRoot") && !starts_with(startState->name, "ProperRoot") && start_with_vowel_or_consonant_drops(transition)) {
                        //---lastIdropsDuringSuffixation---
                        if (soften_during_suffixation(transition, root)) {
                            //---softenDuringSuffixation---
                            if (strcmp(last_p, "p") == 0){
                                //hizip->hizbi, kayıp->kaybı, kayıt->kaydı, kutup->kutbu
                                free_string_ptr(formation);
                                formation = create_string3(except_last_two, "b");
                            } else {
                                if (strcmp(last_p, "t") == 0){
                                    //akit->akdi, ahit->ahdi, lahit->lahdi, nakit->nakdi, vecit->vecdi
                                    free_string_ptr(formation);
                                    formation = create_string3(except_last_two, "d");
                                } else {
                                    if (strcmp(last_p, "ç") == 0){
                                        //eviç->evci, nesiç->nesci
                                        free_string_ptr(formation);
                                        formation = create_string3(except_last_two, "c");
                                    }
                                }
                            }
                        } else {
                            //sarıağız->sarıağzı, zehir->zehri, zikir->zikri, nutuk->nutku, omuz->omzu, ömür->ömrü
                            //lütuf->lütfu, metin->metni, kavim->kavmi, kasıt->kastı
                            free_string_ptr(formation);
                            formation = create_string3(except_last_two, last);
                        }
                        formation_to_check = create_string2(stem);
                        formation_to_check_allocated = true;
                    } else {
                        //---nounSoftenDuringSuffixation or verbSoftenDuringSuffixation
                        if (strcmp(last_p, "p") == 0){
                            //adap->adabı, amip->amibi, azap->azabı, gazap->gazabı
                            if (start_with_vowel_or_consonant_drops(transition) && rootWord && soften_during_suffixation(transition, root)) {
                                free_string_ptr(formation);
                                formation = create_string3(except_last, "b");
                            }
                        } else {
                            if (strcmp(last_p, "t") == 0){
                                //abat->abadı, adet->adedi, akort->akordu, armut->armudu
                                //affet->affedi, yoket->yokedi, sabret->sabredi, rakset->raksedi
                                if (start_with_vowel_or_consonant_drops(transition) && rootWord && soften_during_suffixation(transition, root)) {
                                    free_string_ptr(formation);
                                    formation = create_string3(except_last, "d");
                                }
                            } else {
                                if (strcmp(last_p, "ç") == 0){
                                    //ağaç->ağacı, almaç->almacı, akaç->akacı, avuç->avucu
                                    if (start_with_vowel_or_consonant_drops(transition) && rootWord && soften_during_suffixation(transition, root)) {
                                        free_string_ptr(formation);
                                        formation = create_string3(except_last, "c");
                                    }
                                } else {
                                    if (strcmp(last_p, "g") == 0){
                                        //arkeolog->arkeoloğu, filolog->filoloğu, minerolog->mineroloğu
                                        if (start_with_vowel_or_consonant_drops(transition) && rootWord && soften_during_suffixation(transition, root)) {
                                            free_string_ptr(formation);
                                            formation = create_string3(except_last, "ğ");
                                        }
                                    } else {
                                        if (strcmp(last_p, "k") == 0){
                                            //ahenk->ahengi, künk->küngü, renk->rengi, pelesenk->pelesengi
                                            if (start_with_vowel_or_consonant_drops(transition) && rootWord && ending_k_changes_into_g(root) && (!is_proper_noun(root) || strcmp(startState->name, "ProperRoot") != 0)) {
                                                free_string_ptr(formation);
                                                formation = create_string3(except_last, "g");
                                            } else {
                                                //ablak->ablağı, küllük->küllüğü, kitaplık->kitaplığı, evcilik->evciliği
                                                if (start_with_vowel_or_consonant_drops(transition) && (!rootWord || (soften_during_suffixation(transition, root) && (!is_proper_noun(root) || strcmp(startState->name, "ProperRoot") != 0)))) {
                                                    free_string_ptr(formation);
                                                    formation = create_string3(except_last, "ğ");
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                        formation_to_check = formation;
                    }
                }
            }
        }
    }
    free_string_ptr(with_zero);
    free_string_ptr(with_first);
    Array_list_ptr withChars = all_characters(transition->with);
    String_ptr st0 = array_list_get(withChars, 0);
    if (is_consonant_drop(with_f) && !is_vowel(last) && (is_numeral(root) || is_real(root) ||
            is_fraction(root) || is_time2(root) || is_date(root) || is_percent(root) || is_range(root)) && (ends_with(root->name, "1") || ends_with(root->name, "3") || ends_with(root->name, "4") || ends_with(root->name, "5") || ends_with(root->name, "8") || ends_with(root->name, "9") || ends_with(root->name, "10") || ends_with(root->name, "30") || ends_with(root->name, "40") || ends_with(root->name, "60") || ends_with(root->name, "70") || ends_with(root->name, "80") || ends_with(root->name, "90") || ends_with(root->name, "00"))) {
        if (string_equals2(st0, "'")) {
            string_append(formation, "'");
            i = 2;
        } else {
            i = 1;
        }
    } else {
        if ((is_consonant_drop(with_f) && is_consonant(last_p)) || (rootWord && consonant_s_may_inserted_during_possesive_suffixation(root))) {
            if (string_equals2(st0, "'")) {
                string_append(formation, "'");
                if (is_abbreviation(root))
                    i = 1;
                else
                    i = 2;
            } else {
                i = 1;
            }
        }
    }
    for (; i < word_size(transition->with); i++) {
        String_ptr sti = array_list_get(withChars, i);
        if (string_equals2(sti, "D")){
            resolve_D(root, formation, formation_to_check->s);
        } else {
            if (string_equals2(sti, "A")){
                resolve_A(root, formation, rootWord, formation_to_check->s);
            } else {
                if (string_equals2(sti, "H")){
                    if (!string_equals2(st0, "'")) {
                        resolve_H(root, formation, i == 0, starts_with(transition->with, "Hyor"), rootWord, formation_to_check->s);
                    } else {
                        resolve_H(root, formation, i == 1, false, rootWord, formation_to_check->s);
                    }
                    rootWord = false;
                } else {
                    if (string_equals2(sti, "C")){
                        resolve_C(formation, formation_to_check->s);
                    } else {
                        if (string_equals2(sti, "S")){
                            resolve_S(formation);
                        } else {
                            if (string_equals2(sti, "Ş")){
                                resolve_Sh(formation);
                            } else {
                                if (i == word_size(transition->with) - 1 && string_equals2(sti, "s")) {
                                    string_append(formation, "ş");
                                } else {
                                    string_append_s(formation, sti);
                                }
                            }
                        }
                    }
                }
            }
        }
        if (formation_to_check_allocated){
            free_string_ptr(formation_to_check);
            formation_to_check_allocated = false;
        }
        formation_to_check = formation;
    }
    if (formation_to_check_allocated){
        free_string_ptr(formation_to_check);
    }
    free_array_list(withChars, (void (*)(void *)) free_string_ptr);
    result = str_copy(result, formation->s);
    free_string_ptr(formation);
    return result;
}
