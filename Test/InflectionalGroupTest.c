//
// Created by Olcay Taner YILDIZ on 15.10.2023.
//

#include <stdio.h>
#include "../src/InflectionalGroup.h"

void test_get_morphological_tag(){
    if (get_morphological_tag("noun") != NOUN){
        printf("Error in 1\n");
    }
    if (get_morphological_tag("without") != WITHOUT){
        printf("Error in 2\n");
    }
    if (get_morphological_tag("interj") != INTERJECTION){
        printf("Error in 3\n");
    }
    if (get_morphological_tag("inf2") != INFINITIVE2){
        printf("Error in 1\n");
    }
}

void test_size_single(char* group, int size){
    Inflectional_group_ptr ig = create_inflectional_group(group);
    if (ig->IG->size != size){
        printf("Error in %s size %d\n", group, size);
    }
    free_inflectional_group(ig);
}

void test_size(){
    test_size_single("ADJ", 1);
    test_size_single("ADJ+JUSTLIKE", 2);
    test_size_single("ADJ+FUTPART+P1PL", 3);
    test_size_single("NOUN+A3PL+P1PL+ABL", 4);
    test_size_single("ADJ+WITH+A3SG+P3SG+ABL", 5);
    test_size_single("VERB+ABLE+NEG+FUT+A3PL+COP", 6);
    test_size_single("VERB+ABLE+NEG+AOR+A3SG+COND+A1SG", 7);
}

void test_contains_case_single(char* group){
    Inflectional_group_ptr ig = create_inflectional_group(group);
    if (contains_case(ig) == NOTAG){
        printf("Error in contains case %s\n", group);
    }
    free_inflectional_group(ig);
}

void test_contains_case(){
    test_contains_case_single("NOUN+ACTOF+A3PL+P1PL+NOM");
    test_contains_case_single("NOUN+A3PL+P1PL+ACC");
    test_contains_case_single("NOUN+ZERO+A3SG+P3PL+DAT");
    test_contains_case_single("PRON+QUANTP+A1PL+P1PL+LOC");
    test_contains_case_single("NOUN+AGT+A3SG+P2SG+ABL");
}

void test_contains_plural_single(char* group){
    Inflectional_group_ptr ig = create_inflectional_group(group);
    if (!contains_plural(ig)){
        printf("Error in contains plural %s\n", group);
    }
    free_inflectional_group(ig);
}

void test_contains_plural(){
    test_contains_plural_single("VERB+NEG+NECES+A1PL");
    test_contains_plural_single("PRON+PERS+A2PL+PNON+NOM");
    test_contains_plural_single("NOUN+DIM+A3PL+P2SG+GEN");
    test_contains_plural_single("NOUN+A3PL+P1PL+GEN");
    test_contains_plural_single("NOUN+ZERO+A3SG+P2PL+INS");
    test_contains_plural_single("PRON+QUANTP+A3PL+P3PL+LOC");
}

void test_contains_tag_single(char* group, Morphological_tag tag){
    Inflectional_group_ptr ig = create_inflectional_group(group);
    if (!contains_tag(ig, tag)){
        printf("Error in contains tag %s\n", group);
    }
    free_inflectional_group(ig);
}

void test_contains_tag(){
    test_contains_tag_single("NOUN+ZERO+A3SG+P1SG+NOM", NOUN);
    test_contains_tag_single("NOUN+AGT+A3PL+P2SG+ABL", AGENT);
    test_contains_tag_single("NOUN+INF2+A3PL+P3SG+NOM", NOMINATIVE);
    test_contains_tag_single("NOUN+ZERO+A3SG+P1PL+ACC", ZERO);
    test_contains_tag_single("NOUN+ZERO+A3SG+P2PL+INS", P2PL);
    test_contains_tag_single("PRON+QUANTP+A3PL+P3PL+LOC", QUANTITATIVEPRONOUN);
}

void test_contains_possessive_single(char* group){
    Inflectional_group_ptr ig = create_inflectional_group(group);
    if (!contains_possessive(ig)){
        printf("Error in contains possessive %s\n", group);
    }
    free_inflectional_group(ig);
}

void test_contains_possessive(){
    test_contains_possessive_single("NOUN+ZERO+A3SG+P1SG+NOM");
    test_contains_possessive_single("NOUN+AGT+A3PL+P2SG+ABL");
    test_contains_possessive_single("NOUN+INF2+A3PL+P3SG+NOM");
    test_contains_possessive_single("NOUN+ZERO+A3SG+P1PL+ACC");
    test_contains_possessive_single("NOUN+ZERO+A3SG+P2PL+INS");
    test_contains_possessive_single("PRON+QUANTP+A3PL+P3PL+LOC");
}

int main(){
    test_get_morphological_tag();
    test_size();
    test_contains_case();
    test_contains_plural();
    test_contains_tag();
    test_contains_possessive();
}