//
// Created by Olcay Taner YILDIZ on 5.11.2023.
//

#include <stdlib.h>
#include "../src/FsmMorphologicalAnalyzer.h"
#include "../src/Transition.h"

void test_analysis(Fsm_morphological_analyzer_ptr fsm, char* list[], int size){
    for (int i = 0; i < size; i++){
        Fsm_parse_list_ptr parse_list = morphological_analysis(fsm, list[i]);
        if (parse_list->fsm_parses->size == 0){
            printf("Error in word %s\n", list[i]);
        }
        free_fsm_parse_list(parse_list);
    }
}

void test_robust_analysis(Fsm_morphological_analyzer_ptr fsm, char* list[], int size){
    for (int i = 0; i < size; i++){
        Fsm_parse_list_ptr parse_list = robust_morphological_analysis(fsm, list[i]);
        if (parse_list->fsm_parses->size == 0){
            printf("Error in word %s\n", list[i]);
        }
        free_fsm_parse_list(parse_list);
    }
}

void test_replace_word_single(Fsm_morphological_analyzer_ptr fsm, char* sentence, char* previous, char* new, char* new_sentence){
    Sentence_ptr s1 = create_sentence3(sentence);
    Sentence_ptr s2 = replace_word_fsm(fsm, s1, previous, new);
    String_ptr st = to_words(s2);
    if (!string_equals2(st, new_sentence)){
        printf("Error sentence %s should be equal to %s\n", new_sentence, st->s);
    }
    free_string_ptr(st);
    free_sentence(s1);
    free(s2);
}

void test_replace_word(Fsm_morphological_analyzer_ptr fsm){
    test_replace_word_single(fsm, "Kız kardeşinle evlendim", "kız kardeş", "hemşire", "Hemşirenle evlendim");
    test_replace_word_single(fsm, "Yemin billah vermişlerdi vazoyu kırmadığına", "yemin billah ver", "yemin et", "Yemin etmişlerdi vazoyu kırmadığına");
    test_replace_word_single(fsm, "Burada çok mübalağa var", "mübalağa", "abartma", "Burada çok abartma var");
    test_replace_word_single(fsm, "Bu bina çok kötü biçimsizleştirildi", "biçimsizleş", "şekilsizleş", "Bu bina çok kötü şekilsizleştirildi");
    test_replace_word_single(fsm, "Hemşirenle evlendim", "hemşire", "kız kardeş", "Kız kardeşinle evlendim");
    test_replace_word_single(fsm, "Abim geçen yıl son yolculuğa çıkmıştı gibi", "son yolculuğa çık", "öl", "Abim geçen yıl ölmüştü gibi");
    test_replace_word_single(fsm, "Dün yaptığı güreş maçında mağlup oldu", "mağlup ol", "yenil", "Dün yaptığı güreş maçında yenildi");
    test_replace_word_single(fsm, "Abim geçen yıl ölmüştü gibi", "öl", "son yolculuğa çık", "Abim geçen yıl son yolculuğa çıkmıştı gibi");
    test_replace_word_single(fsm, "Dün yaptığı güreş maçında yenildi", "yenil", "mağlup ol", "Dün yaptığı güreş maçında mağlup oldu");
    test_replace_word_single(fsm, "Dün yaptığı güreş maçında mağlup oldu sanki", "mağlup ol", "alt ol", "Dün yaptığı güreş maçında alt oldu sanki");
    test_replace_word_single(fsm, "Yemin etmişlerdi vazoyu kırmadığına", "yemin et", "yemin billah ver", "Yemin billah vermişlerdi vazoyu kırmadığına");
    test_replace_word_single(fsm, "Hemşirene söyle kazağı güzelmiş", "hemşire", "şvester", "Şvesterine söyle kazağı güzelmiş");
}

void test_tags(Fsm_morphological_analyzer_ptr fsm){
    Fsm_State_ptr transition_state, start_state;
    Transition_ptr transition;
    char* surface_form;
    int count = 0;
    for (int i = 0; i < fsm->dictionary->dictionary->words->size; i++){
        surface_form = NULL;
        Txt_word_ptr word = get_word_with_index_txt(fsm->dictionary, i);
        if (is_nominal(word) && noun_soften_during_suffixation(word)){
            transition_state = create_fsm_state("Possessive", false, false);
            start_state = create_fsm_state("NominalRoot", true, false);
            transition = create_transition(transition_state, "yH", "ACC");
            surface_form = make_transition2(transition, word, word->word->name, start_state);
            count++;
        } else {
            if (is_verb(word) && vowel_a_changes_to_i_during_y_suffixation(word)){
                transition_state = create_fsm_state("VerbalStem", false, false);
                start_state = create_fsm_state("VerbalRoot", true, false);
                transition = create_transition(transition_state, "Hyor", "PROG1");
                surface_form = make_transition2(transition, word, word->word->name, start_state);
                count++;
            } else {
                if (is_nominal(word) && last_i_drops_during_suffixation(word)){
                    transition_state = create_fsm_state("Possessive", false, false);
                    start_state = create_fsm_state("NominalRoot", true, false);
                    transition = create_transition(transition_state, "yH", "ACC");
                    surface_form = make_transition2(transition, word, word->word->name, start_state);
                    count++;
                } else {
                    if (is_verb(word) && verb_soften_during_suffixation(word)){
                        transition_state = create_fsm_state("VerbalStem", false, false);
                        start_state = create_fsm_state("VerbalRoot", true, false);
                        transition = create_transition(transition_state, "Hyor", "PROG1");
                        surface_form = make_transition2(transition, word, word->word->name, start_state);
                        count++;
                    } else {
                        if (is_nominal(word) && duplicates_during_suffixation(word)){
                            transition_state = create_fsm_state("Possessive", false, false);
                            start_state = create_fsm_state("NominalRoot", true, false);
                            transition = create_transition(transition_state, "yH", "ACC");
                            surface_form = make_transition2(transition, word, word->word->name, start_state);
                            count++;
                        } else {
                            if (is_nominal(word) && ending_k_changes_into_g(word)){
                                transition_state = create_fsm_state("Possessive", false, false);
                                start_state = create_fsm_state("NominalRoot", true, false);
                                transition = create_transition(transition_state, "yH", "ACC");
                                surface_form = make_transition2(transition, word, word->word->name, start_state);
                                count++;
                            } else {
                                if (is_verb(word) && last_i_drops_during_passive_suffixation(word)){
                                    transition_state = create_fsm_state("VerbalStem", false, false);
                                    start_state = create_fsm_state("VerbalRoot", true, false);
                                    transition = create_transition(transition_state, "Hl", "^DB+VERB+PASS");
                                    surface_form = make_transition2(transition, word, word->word->name, start_state);
                                    count++;
                                } else {
                                    if (is_nominal(word) && not_obeys_vowel_harmony_during_agglutination(word)){
                                        transition_state = create_fsm_state("Possessive", false, false);
                                        start_state = create_fsm_state("NominalRoot", true, false);
                                        transition = create_transition(transition_state, "yH", "ACC");
                                        surface_form = make_transition2(transition, word, word->word->name, start_state);
                                        count++;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        if (surface_form != NULL){
            Fsm_parse_list_ptr parse_list = morphological_analysis(fsm, surface_form);
            if (parse_list->fsm_parses->size == 0){
                printf("Error in word %s\n", word->word->name);
            }
        }
    }
    printf("Checked %d cases\n", count);
}

int main(){
    Fsm_morphological_analyzer_ptr fsm = create_fsm_morphological_analyzer3();
    test_robust_analysis(fsm, (char*[]){"googlecılardan", "zaptıraplaştırılmayana", "abzürtleşenmiş", "vışlığından"}, 4);
    test_analysis(fsm, (char*[]){"Times'ın", "Times'tır", "Times'mış", "Twitter'ın", "Twitter'dır", "Twitter'mış"}, 6);
    test_analysis(fsm, (char*[]){"3/4", "3\\/4", "4/2/1973", "14/2/1993", "14/12/1933", "6/12/1903", "%34.5", "%3", "%56", "2:3", "12:3", "4:23", "11:56", "1:2:3", "3:12:3", "5:4:23", "7:11:56", "12:2:3", "10:12:3", "11:4:23", "22:11:56", "34.23"}, 22);
    test_replace_word(fsm);
    test_tags(fsm);
    free_fsm_morphological_analyzer(fsm);
}