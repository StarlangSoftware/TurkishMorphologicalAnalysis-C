//
// Created by Olcay Taner YILDIZ on 2.11.2023.
//

#include <string.h>
#include <stdlib.h>
#include "../src/FsmMorphologicalAnalyzer.h"

void test_size_single(Fsm_morphological_analyzer_ptr fsm, char* word, int size){
    Fsm_parse_list_ptr parse_list = morphological_analysis(fsm, word);
    if (parse_list->fsm_parses->size != size){
        printf("Error test size in word %s %d\n", word, parse_list->fsm_parses->size);
    }
    free_fsm_parse_list(parse_list);
}

void test_size(Fsm_morphological_analyzer_ptr fsm){
    test_size_single(fsm, "açılır", 2);
    test_size_single(fsm, "koparılarak", 2);
    test_size_single(fsm, "toplama", 6);
    test_size_single(fsm, "değerlendirmede", 4);
    test_size_single(fsm, "soruşturmasının", 5);
    test_size_single(fsm, "karşılaştırmalı", 12);
    test_size_single(fsm, "esaslarını", 8);
    test_size_single(fsm, "güçleriyle", 6);
    test_size_single(fsm, "bulmayacakları", 5);
    test_size_single(fsm, "sana", 4);
}

void test_root_words_single(Fsm_morphological_analyzer_ptr fsm, char* word, char* roots){
    Fsm_parse_list_ptr parse_list = morphological_analysis(fsm, word);
    char* root = root_words(parse_list);
    if (strcmp(root, roots) != 0){
        printf("Error test root words in word %s\n", word);
    }
    free(root);
    free_fsm_parse_list(parse_list);
}

void test_root_words(Fsm_morphological_analyzer_ptr fsm){
    test_root_words_single(fsm, "açılır", "aç");
    test_root_words_single(fsm, "koparılarak", "kop$kopar");
    test_root_words_single(fsm, "toplama", "topla$toplam$toplama");
    test_root_words_single(fsm, "değerlendirmede", "değer$değerlen$değerlendir$değerlendirme");
    test_root_words_single(fsm, "soruşturmasının", "sor$soru$soruş$soruştur$soruşturma");
    test_root_words_single(fsm, "karşılaştırmalı", "karşı$karşıla$karşılaş$karşılaştır$karşılaştırma$karşılaştırmalı");
    test_root_words_single(fsm, "esaslarını", "esas");
    test_root_words_single(fsm, "güçleriyle", "güç");
    test_root_words_single(fsm, "bulmayacakları", "bul");
}

void test_get_parse_with_longest_root_word_single(Fsm_morphological_analyzer_ptr fsm, char* word, char* roots){
    Fsm_parse_list_ptr parse_list = morphological_analysis(fsm, word);
    if (strcmp(get_parse_with_longest_root_word(parse_list)->root->name, roots) != 0){
        printf("Error test longest root word in word %s\n", word);
    }
    free_fsm_parse_list(parse_list);
}

void test_get_parse_with_longest_root_word(Fsm_morphological_analyzer_ptr fsm){
    test_get_parse_with_longest_root_word_single(fsm, "koparılarak", "kopar");
    test_get_parse_with_longest_root_word_single(fsm, "toplama", "toplama");
    test_get_parse_with_longest_root_word_single(fsm, "değerlendirmede", "değerlendirme");
    test_get_parse_with_longest_root_word_single(fsm, "soruşturmasının", "soruşturma");
    test_get_parse_with_longest_root_word_single(fsm, "karşılaştırmalı", "karşılaştırmalı");
    test_get_parse_with_longest_root_word_single(fsm, "açacağını", "aç");
    test_get_parse_with_longest_root_word_single(fsm, "kollarımız", "kol");
    test_get_parse_with_longest_root_word_single(fsm, "yapmamızı", "yap");
    test_get_parse_with_longest_root_word_single(fsm, "koşmalıyız", "koş");
}

void test_reduce_to_parses_with_same_root_and_pos_single(Fsm_morphological_analyzer_ptr fsm, char* word, char* root_and_pos, int size){
    Fsm_parse_list_ptr parse_list = morphological_analysis(fsm, word);
    reduce_to_parses_with_same_root_and_pos(parse_list, create_txt_word(root_and_pos));
    if (parse_list->fsm_parses->size != size){
        printf("Error test reduce parses root and pos in word %s\n", word);
    }
    free_fsm_parse_list(parse_list);
}

void test_reduce_to_parses_with_same_root_and_pos(Fsm_morphological_analyzer_ptr fsm) {
    test_reduce_to_parses_with_same_root_and_pos_single(fsm, "koparılarak", "kop+VERB", 1);
    test_reduce_to_parses_with_same_root_and_pos_single(fsm, "toplama", "topla+VERB", 2);
    test_reduce_to_parses_with_same_root_and_pos_single(fsm, "karşılaştırmalı", "karşıla+VERB", 2);
}

void test_reduce_to_parses_with_same_root_single(Fsm_morphological_analyzer_ptr fsm, char* word, char* root, int size){
    Fsm_parse_list_ptr parse_list = morphological_analysis(fsm, word);
    reduce_to_parses_with_same_root(parse_list, root);
    if (parse_list->fsm_parses->size != size){
        printf("Error test reduce parses root in word %s\n", word);
    }
    free_fsm_parse_list(parse_list);
}

void test_reduce_to_parses_with_same_root(Fsm_morphological_analyzer_ptr fsm) {
    test_reduce_to_parses_with_same_root_single(fsm, "koparılarak", "kop", 1);
    test_reduce_to_parses_with_same_root_single(fsm, "toplama", "topla", 3);
    test_reduce_to_parses_with_same_root_single(fsm, "karşılaştırmalı", "karşı", 4);
    test_reduce_to_parses_with_same_root_single(fsm, "esaslarını", "esas", 8);
    test_reduce_to_parses_with_same_root_single(fsm, "güçleriyle", "güç", 6);
}

void test_construct_parse_list_for_different_root_with_pos_single(Fsm_morphological_analyzer_ptr fsm, char* word, int size) {
    Fsm_parse_list_ptr parse_list = morphological_analysis(fsm, word);
    Array_list_ptr list = construct_parse_list_for_different_root_with_pos(parse_list);
    if (list->size != size){
        printf("Error test different root pos in word %s\n", word);
    }
    free_array_list(list, NULL);
    free_fsm_parse_list(parse_list);
}

void test_construct_parse_list_for_different_root_with_pos(Fsm_morphological_analyzer_ptr fsm) {
    test_construct_parse_list_for_different_root_with_pos_single(fsm, "açılır", 1);
    test_construct_parse_list_for_different_root_with_pos_single(fsm, "koparılarak", 2);
    test_construct_parse_list_for_different_root_with_pos_single(fsm, "toplama", 5);
    test_construct_parse_list_for_different_root_with_pos_single(fsm, "değerlendirmede", 4);
    test_construct_parse_list_for_different_root_with_pos_single(fsm, "soruşturmasının", 5);
    test_construct_parse_list_for_different_root_with_pos_single(fsm, "karşılaştırmalı", 7);
    test_construct_parse_list_for_different_root_with_pos_single(fsm, "esaslarını", 2);
    test_construct_parse_list_for_different_root_with_pos_single(fsm, "güçleriyle", 2);
    test_construct_parse_list_for_different_root_with_pos_single(fsm, "bulmayacakları", 1);
}

void test_parses_without_prefix_and_suffix_single(Fsm_morphological_analyzer_ptr fsm, char* word, char* parse) {
    Fsm_parse_list_ptr parse_list = morphological_analysis(fsm, word);
    char* parse_without = parses_without_prefix_and_suffix(parse_list);
    if (strcmp(parse_without, parse) != 0){
        printf("Error test without prefix and suffix in word %s %s\n", word, parse_without);
    }
    free(parse_without);
    free_fsm_parse_list(parse_list);
}

void test_parses_without_prefix_and_suffix(Fsm_morphological_analyzer_ptr fsm){
    test_parses_without_prefix_and_suffix_single(fsm, "kitabı", "P3SG+NOM$PNON+ACC");
    test_parses_without_prefix_and_suffix_single(fsm, "kitapları", "A3PL+P3PL+NOM$A3PL+P3SG+NOM$A3PL+PNON+ACC$A3SG+P3PL+NOM");
    test_parses_without_prefix_and_suffix_single(fsm, "o", "DET$PRON+DEMONSP+A3SG+PNON+NOM$PRON+PERS+A3SG+PNON+NOM");
    test_parses_without_prefix_and_suffix_single(fsm, "arabası", "NOUN+A3SG+P3SG+NOM$NOUN^DB+ADJ+ALMOST");
}

int main(){
    Fsm_morphological_analyzer_ptr fsm = create_fsm_morphological_analyzer3();
    test_size(fsm);
    test_root_words(fsm);
    test_get_parse_with_longest_root_word(fsm);
    test_reduce_to_parses_with_same_root_and_pos(fsm);
    test_reduce_to_parses_with_same_root(fsm);
    test_construct_parse_list_for_different_root_with_pos(fsm);
    free_fsm_morphological_analyzer(fsm);
}

