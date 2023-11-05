//
// Created by Olcay Taner YILDIZ on 18.10.2023.
//

#include <string.h>
#include <stdio.h>
#include "../src/MorphologicalParse.h"

void test_get_transition_list_single(char* parse_string, char* list){
    Morphological_parse_ptr parse1 = create_morphological_parse(parse_string);
    if (strcmp(get_transition_list(parse1), list) != 0){
        printf("Error transition list %s\n", parse_string);
    }
    free_morphological_parse(parse1);
}

void test_get_transition_list(){
    test_get_transition_list_single("bayan+NOUN+A3SG+PNON+NOM", "NOUN+A3SG+PNON+NOM");
    test_get_transition_list_single("yaşa+VERB+POS^DB+ADJ+PRESPART", "VERB+POS+ADJ+PRESPART");
    test_get_transition_list_single("serbest+ADJ", "ADJ");
    test_get_transition_list_single("et+VERB^DB+VERB+PASS^DB+VERB+ABLE+NEG+AOR+A3SG", "VERB+VERB+PASS+VERB+ABLE+NEG+AOR+A3SG");
    test_get_transition_list_single("sür+VERB^DB+VERB+CAUS^DB+VERB+PASS+POS^DB+NOUN+INF2+A3SG+P3SG+NOM", "VERB+VERB+CAUS+VERB+PASS+POS+NOUN+INF2+A3SG+P3SG+NOM");
    test_get_transition_list_single("değiş+VERB^DB+VERB+CAUS^DB+VERB+PASS+POS^DB+VERB+ABLE+AOR^DB+ADJ+ZERO", "VERB+VERB+CAUS+VERB+PASS+POS+VERB+ABLE+AOR+ADJ+ZERO");
    test_get_transition_list_single("iyi+ADJ^DB+VERB+BECOME^DB+VERB+CAUS^DB+VERB+PASS+POS^DB+VERB+ABLE^DB+NOUN+INF2+A3PL+P3PL+ABL", "ADJ+VERB+BECOME+VERB+CAUS+VERB+PASS+POS+VERB+ABLE+NOUN+INF2+A3PL+P3PL+ABL");
    test_get_transition_list_single("değil+ADJ^DB+VERB+ZERO+PAST+A3SG", "ADJ+VERB+ZERO+PAST+A3SG");
}

void test_get_tag_single(char* parse_string, int index, char* tag){
    Morphological_parse_ptr parse1 = create_morphological_parse(parse_string);
    if (strcmp(get_tag_for_index(parse1, index), tag) != 0){
        printf("Error get tag %s\n", parse_string);
    }
    free_morphological_parse(parse1);
}

void test_get_tag(){
    test_get_tag_single("bayan+NOUN+A3SG+PNON+NOM", 2, "A3SG");
    test_get_tag_single("yaşa+VERB+POS^DB+ADJ+PRESPART", 4, "PRESPART");
    test_get_tag_single("serbest+ADJ", 0, "serbest");
    test_get_tag_single("et+VERB^DB+VERB+PASS^DB+VERB+ABLE+NEG+AOR+A3SG", 7, "AOR");
    test_get_tag_single("sür+VERB^DB+VERB+CAUS^DB+VERB+PASS+POS^DB+NOUN+INF2+A3SG+P3SG+NOM", 10, "P3SG");
    test_get_tag_single("değiş+VERB^DB+VERB+CAUS^DB+VERB+PASS+POS^DB+VERB+ABLE+AOR^DB+ADJ+ZERO", 8, "ABLE");
    test_get_tag_single("iyi+ADJ^DB+VERB+BECOME^DB+VERB+CAUS^DB+VERB+PASS+POS^DB+VERB+ABLE^DB+NOUN+INF2+A3PL+P3PL+ABL", 15, "ABL");
}

void test_get_tag_size_single(char* parse_string, int size){
    Morphological_parse_ptr parse1 = create_morphological_parse(parse_string);
    if (tag_size(parse1) != size){
        printf("Error tag size %s\n", parse_string);
    }
    free_morphological_parse(parse1);
}

void test_get_tag_size(){
    test_get_tag_size_single("bayan+NOUN+A3SG+PNON+NOM", 5);
    test_get_tag_size_single("yaşa+VERB+POS^DB+ADJ+PRESPART", 5);
    test_get_tag_size_single("serbest+ADJ", 2);
    test_get_tag_size_single("et+VERB^DB+VERB+PASS^DB+VERB+ABLE+NEG+AOR+A3SG", 9);
    test_get_tag_size_single("sür+VERB^DB+VERB+CAUS^DB+VERB+PASS+POS^DB+NOUN+INF2+A3SG+P3SG+NOM", 12);
    test_get_tag_size_single("değiş+VERB^DB+VERB+CAUS^DB+VERB+PASS+POS^DB+VERB+ABLE+AOR^DB+ADJ+ZERO", 12);
    test_get_tag_size_single("iyi+ADJ^DB+VERB+BECOME^DB+VERB+CAUS^DB+VERB+PASS+POS^DB+VERB+ABLE^DB+NOUN+INF2+A3PL+P3PL+ABL", 16);
    test_get_tag_size_single("değil+ADJ^DB+VERB+ZERO+PAST+A3SG", 6);
}

void test_get_root_pos_single(char* parse_string, char* pos){
    Morphological_parse_ptr parse1 = create_morphological_parse(parse_string);
    if (strcmp(get_root_pos(parse1), pos) != 0){
        printf("Error root pos %s\n", parse_string);
    }
    free_morphological_parse(parse1);
}

void test_get_root_pos(){
    test_get_root_pos_single("bayan+NOUN+A3SG+PNON+NOM", "NOUN");
    test_get_root_pos_single("yaşa+VERB+POS^DB+ADJ+PRESPART", "VERB");
    test_get_root_pos_single("serbest+ADJ", "ADJ");
    test_get_root_pos_single("et+VERB^DB+VERB+PASS^DB+VERB+ABLE+NEG+AOR+A3SG", "VERB");
    test_get_root_pos_single("sür+VERB^DB+VERB+CAUS^DB+VERB+PASS+POS^DB+NOUN+INF2+A3SG+P3SG+NOM", "VERB");
    test_get_root_pos_single("değiş+VERB^DB+VERB+CAUS^DB+VERB+PASS+POS^DB+VERB+ABLE+AOR^DB+ADJ+ZERO", "VERB");
    test_get_root_pos_single("iyi+ADJ^DB+VERB+BECOME^DB+VERB+CAUS^DB+VERB+PASS+POS^DB+VERB+ABLE^DB+NOUN+INF2+A3PL+P3PL+ABL", "ADJ");
    test_get_root_pos_single("değil+ADJ^DB+VERB+ZERO+PAST+A3SG", "ADJ");
}

void test_get_pos_single(char* parse_string, char* pos){
    Morphological_parse_ptr parse1 = create_morphological_parse(parse_string);
    if (strcmp(get_pos(parse1), pos) != 0){
        printf("Error pos %s\n", parse_string);
    }
    free_morphological_parse(parse1);
}

void test_get_pos(){
    test_get_pos_single("bayan+NOUN+A3SG+PNON+NOM", "NOUN");
    test_get_pos_single("yaşa+VERB+POS^DB+ADJ+PRESPART", "ADJ");
    test_get_pos_single("serbest+ADJ", "ADJ");
    test_get_pos_single("et+VERB^DB+VERB+PASS^DB+VERB+ABLE+NEG+AOR+A3SG", "VERB");
    test_get_pos_single("sür+VERB^DB+VERB+CAUS^DB+VERB+PASS+POS^DB+NOUN+INF2+A3SG+P3SG+NOM", "NOUN");
    test_get_pos_single("değiş+VERB^DB+VERB+CAUS^DB+VERB+PASS+POS^DB+VERB+ABLE+AOR^DB+ADJ+ZERO", "ADJ");
    test_get_pos_single("iyi+ADJ^DB+VERB+BECOME^DB+VERB+CAUS^DB+VERB+PASS+POS^DB+VERB+ABLE^DB+NOUN+INF2+A3PL+P3PL+ABL", "NOUN");
    test_get_pos_single("değil+ADJ^DB+VERB+ZERO+PAST+A3SG", "VERB");
}

void test_get_word_with_pos_single(char* parse_string, char* pos){
    Morphological_parse_ptr parse1 = create_morphological_parse(parse_string);
    if (strcmp(get_word_with_pos(parse1)->name, pos) != 0){
        printf("Error pos %s\n", parse_string);
    }
    free_morphological_parse(parse1);
}

void test_get_word_with_pos(){
    test_get_word_with_pos_single("bayan+NOUN+A3SG+PNON+NOM", "bayan+NOUN");
    test_get_word_with_pos_single("yaşa+VERB+POS^DB+ADJ+PRESPART", "yaşa+VERB");
    test_get_word_with_pos_single("serbest+ADJ", "serbest+ADJ");
    test_get_word_with_pos_single("et+VERB^DB+VERB+PASS^DB+VERB+ABLE+NEG+AOR+A3SG", "et+VERB");
    test_get_word_with_pos_single("sür+VERB^DB+VERB+CAUS^DB+VERB+PASS+POS^DB+NOUN+INF2+A3SG+P3SG+NOM", "sür+VERB");
    test_get_word_with_pos_single("değiş+VERB^DB+VERB+CAUS^DB+VERB+PASS+POS^DB+VERB+ABLE+AOR^DB+ADJ+ZERO", "değiş+VERB");
    test_get_word_with_pos_single("iyi+ADJ^DB+VERB+BECOME^DB+VERB+CAUS^DB+VERB+PASS+POS^DB+VERB+ABLE^DB+NOUN+INF2+A3PL+P3PL+ABL", "iyi+ADJ");
    test_get_word_with_pos_single("değil+ADJ^DB+VERB+ZERO+PAST+A3SG", "değil+ADJ");
}

void test_get_tree_pos_single(char* parse_string, char* pos){
    Morphological_parse_ptr parse1 = create_morphological_parse(parse_string);
    if (strcmp(get_tree_pos(parse1), pos) != 0){
        printf("Error tree pos %s\n", parse_string);
    }
    free_morphological_parse(parse1);
}

void test_get_tree_pos(){
    test_get_tree_pos_single("bayan+NOUN+A3SG+PNON+NOM", "NP");
    test_get_tree_pos_single("yaşa+VERB+POS^DB+ADJ+PRESPART", "ADJP");
    test_get_tree_pos_single("serbest+ADJ", "ADJP");
    test_get_tree_pos_single("et+VERB^DB+VERB+PASS^DB+VERB+ABLE+NEG+AOR+A3SG", "VP");
    test_get_tree_pos_single("sür+VERB^DB+VERB+CAUS^DB+VERB+PASS+POS^DB+NOUN+INF2+A3SG+P3SG+NOM", "NP");
    test_get_tree_pos_single("değiş+VERB^DB+VERB+CAUS^DB+VERB+PASS+POS^DB+VERB+ABLE+AOR^DB+ADJ+ZERO", "ADJP");
    test_get_tree_pos_single("iyi+ADJ^DB+VERB+BECOME^DB+VERB+CAUS^DB+VERB+PASS+POS^DB+VERB+ABLE^DB+NOUN+INF2+A3PL+P3PL+ABL", "NP");
    test_get_tree_pos_single("değil+ADJ^DB+VERB+ZERO+PAST+A3SG", "NEG");
}

void test_is_root_verb_single(char* parse_string){
    Morphological_parse_ptr parse1 = create_morphological_parse(parse_string);
    if (!is_root_verb(parse1)){
        printf("Error is root verb %s\n", parse_string);
    }
    free_morphological_parse(parse1);
}

void test_is_root_verb(){
    test_is_root_verb_single("yaşa+VERB+POS^DB+ADJ+PRESPART");
    test_is_root_verb_single("et+VERB^DB+VERB+PASS^DB+VERB+ABLE+NEG+AOR+A3SG");
    test_is_root_verb_single("sür+VERB^DB+VERB+CAUS^DB+VERB+PASS+POS^DB+NOUN+INF2+A3SG+P3SG+NOM");
    test_is_root_verb_single("değiş+VERB^DB+VERB+CAUS^DB+VERB+PASS+POS^DB+VERB+ABLE+AOR^DB+ADJ+ZERO");
}

void test_is_verb_single(char* parse_string){
    Morphological_parse_ptr parse1 = create_morphological_parse(parse_string);
    if (!is_parse_verb(parse1)){
        printf("Error is verb %s\n", parse_string);
    }
    free_morphological_parse(parse1);
}

void test_is_verb(){
    test_is_verb_single("et+VERB^DB+VERB+PASS^DB+VERB+ABLE+NEG+AOR+A3SG");
    test_is_verb_single("değil+ADJ^DB+VERB+ZERO+PAST+A3SG");
}

void test_is_adjective_single(char* parse_string){
    Morphological_parse_ptr parse1 = create_morphological_parse(parse_string);
    if (!is_parse_adjective(parse1)){
        printf("Error is adjective %s\n", parse_string);
    }
    free_morphological_parse(parse1);
}

void test_is_adjective(){
    test_is_adjective_single("yaşa+VERB+POS^DB+ADJ+PRESPART");
    test_is_adjective_single("serbest+ADJ");
    test_is_adjective_single("değiş+VERB^DB+VERB+CAUS^DB+VERB+PASS+POS^DB+VERB+ABLE+AOR^DB+ADJ+ZERO");
}

void test_is_noun_single(char* parse_string){
    Morphological_parse_ptr parse1 = create_morphological_parse(parse_string);
    if (!is_noun(parse1)){
        printf("Error is noun %s\n", parse_string);
    }
    free_morphological_parse(parse1);
}

void test_is_noun(){
    test_is_noun_single("bayan+NOUN+A3SG+PNON+NOM");
    test_is_noun_single("sür+VERB^DB+VERB+CAUS^DB+VERB+PASS+POS^DB+NOUN+INF2+A3SG+P3SG+NOM");
    test_is_noun_single("iyi+ADJ^DB+VERB+BECOME^DB+VERB+CAUS^DB+VERB+PASS+POS^DB+VERB+ABLE^DB+NOUN+INF2+A3PL+P3PL+ABL");
}

void test_is_plural_single(char* parse_string){
    Morphological_parse_ptr parse1 = create_morphological_parse(parse_string);
    if (is_parse_plural(parse1)){
        printf("Error is plural %s\n", parse_string);
    }
    free_morphological_parse(parse1);
}

void test_is_plural(){
    test_is_plural_single("bayan+NOUN+A3SG+PNON+NOM");
    test_is_plural_single("yaşa+VERB+POS^DB+ADJ+PRESPART");
    test_is_plural_single("serbest+ADJ");
    test_is_plural_single("et+VERB^DB+VERB+PASS^DB+VERB+ABLE+NEG+AOR+A3SG");
    test_is_plural_single("sür+VERB^DB+VERB+CAUS^DB+VERB+PASS+POS^DB+NOUN+INF2+A3SG+P3SG+NOM");
    test_is_plural_single("değiş+VERB^DB+VERB+CAUS^DB+VERB+PASS+POS^DB+VERB+ABLE+AOR^DB+ADJ+ZERO");
}

int main(){
    test_get_transition_list();
    test_get_tag();
    test_get_tag_size();
    test_get_root_pos();
    test_get_pos();
    test_get_tree_pos();
    test_get_word_with_pos();
    test_is_root_verb();
    test_is_verb();
    test_is_adjective();
    test_is_noun();
    test_is_plural();
}