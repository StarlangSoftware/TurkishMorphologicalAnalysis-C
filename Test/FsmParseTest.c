//
// Created by Olcay Taner YILDIZ on 2.11.2023.
//

#include <string.h>
#include <Memory/Memory.h>
#include "../src/FsmMorphologicalAnalyzer.h"

void test_get_last_lemma_with_tag_single(Fsm_morphological_analyzer_ptr fsm, int index, char* word, char* tag, char* lemma){
    Fsm_parse_list_ptr parse_list = morphological_analysis(fsm, word);
    Fsm_parse_ptr parse1 = get_fsm_parse(parse_list, index);
    char* s = get_last_lemma_with_tag(parse1, tag);
    if (strcmp(s, lemma) != 0){
        printf("Error in word lemma with tag %s\n", word);
    }
    free_(s);
    free_fsm_parse_list(parse_list);
}

void test_get_last_lemma_with_tag(Fsm_morphological_analyzer_ptr fsm){
    test_get_last_lemma_with_tag_single(fsm, 0, "açılır", "VERB", "açıl");
    test_get_last_lemma_with_tag_single(fsm, 0, "koparılarak", "VERB", "koparıl");
    test_get_last_lemma_with_tag_single(fsm, 0, "soruşturmasının", "VERB", "soruştur");
    test_get_last_lemma_with_tag_single(fsm, 0, "karşılaştırmalı", "ADJ", "karşı");
    test_get_last_lemma_with_tag_single(fsm, 0, "değerlendirmede", "VERB", "değerlendir");
}

void test_get_last_lemma_single(Fsm_morphological_analyzer_ptr fsm, int index, char* word, char* lemma){
    Fsm_parse_list_ptr parse_list = morphological_analysis(fsm, word);
    Fsm_parse_ptr parse1 = get_fsm_parse(parse_list, index);
    char* s = get_last_lemma(parse1);
    if (strcmp(s, lemma) != 0){
        printf("Error in word lemma %s %s\n", word, s);
    }
    free_(s);
    free_fsm_parse_list(parse_list);
}

void test_get_last_lemma(Fsm_morphological_analyzer_ptr fsm){
    test_get_last_lemma_single(fsm, 0, "açılır", "açıl");
    test_get_last_lemma_single(fsm, 0, "koparılarak", "koparılarak");
    test_get_last_lemma_single(fsm, 0, "soruşturmasının", "soruşturma");
    test_get_last_lemma_single(fsm, 0, "karşılaştırmalı", "karşılaştır");
    test_get_last_lemma_single(fsm, 0, "değerlendirmede", "değerlendirme");
}

void test_get_transition_list_single(Fsm_morphological_analyzer_ptr fsm, int index, char* word, char* list){
    Fsm_parse_list_ptr parse_list = morphological_analysis(fsm, word);
    Fsm_parse_ptr parse1 = get_fsm_parse(parse_list, index);
    char* s = transition_list(parse1);
    if (strcmp(s, list) != 0){
        printf("Error in transition list %s %s\n", word, s);
    }
    free_(s);
    free_fsm_parse_list(parse_list);
}

void test_get_transition_list(Fsm_morphological_analyzer_ptr fsm) {
    test_get_transition_list_single(fsm, 0, "açılır", "aç+VERB^DB+VERB+PASS+POS+AOR+A3SG");
    test_get_transition_list_single(fsm, 0, "koparılarak", "kop+VERB^DB+VERB+CAUS^DB+VERB+PASS+POS^DB+ADV+BYDOINGSO");
    test_get_transition_list_single(fsm, 0, "toplama", "topla+NOUN+A3SG+P1SG+DAT");
    test_get_transition_list_single(fsm, 0, "değerlendirmede", "değer+NOUN+A3SG+PNON+NOM^DB+VERB+ACQUIRE^DB+VERB+CAUS+POS^DB+NOUN+INF2+A3SG+PNON+LOC");
    test_get_transition_list_single(fsm, 0, "soruşturmasının", "sor+VERB+RECIP^DB+VERB+CAUS+POS^DB+NOUN+INF2+A3SG+P3SG+GEN");
    test_get_transition_list_single(fsm, 0, "esaslarını", "esas+ADJ^DB+NOUN+ZERO+A3PL+P2SG+ACC");
    test_get_transition_list_single(fsm, 0, "güçleriyle", "güç+ADJ^DB+NOUN+ZERO+A3PL+P3PL+INS");
    test_get_transition_list_single(fsm, 0, "bulmayacakları", "bul+VERB+NEG^DB+ADJ+FUTPART+P3PL");
    test_get_transition_list_single(fsm, 0, "mü", "mi+QUES+PRES+A3SG");
}

void test_with_list_single(Fsm_morphological_analyzer_ptr fsm, int index, char* word, char* list){
    Fsm_parse_list_ptr parse_list = morphological_analysis(fsm, word);
    Fsm_parse_ptr parse1 = get_fsm_parse(parse_list, index);
    char* s = get_with_list(parse1);
    if (strcmp(s, list) != 0){
        printf("Error in with list %s %s\n", word, s);
    }
    free_(s);
    free_fsm_parse_list(parse_list);
}

void test_with_list(Fsm_morphological_analyzer_ptr fsm) {
    test_with_list_single(fsm, 0, "açılır", "aç+Hl+Hr");
    test_with_list_single(fsm, 0, "koparılarak", "kop+Ar+Hl+yArAk");
    test_with_list_single(fsm, 0, "toplama", "topla+Hm+yA");
    test_with_list_single(fsm, 0, "değerlendirmede", "değer+lAn+DHr+mA+DA");
    test_with_list_single(fsm, 0, "soruşturmasının", "sor+Hs+DHr+mA+sH+nHn");
    test_with_list_single(fsm, 0, "esaslarını", "esas+lAr+Hn+yH");
    test_with_list_single(fsm, 0, "güçleriyle", "güç+lArH+ylA");
    test_with_list_single(fsm, 0, "bulmayacakları", "bul+mA+yAcAk+lArH");
}

void test_suffix_list_single(Fsm_morphological_analyzer_ptr fsm, int index, char* word, char* list){
    Fsm_parse_list_ptr parse_list = morphological_analysis(fsm, word);
    Fsm_parse_ptr parse1 = get_fsm_parse(parse_list, index);
    char* s = get_suffix_list(parse1);
    if (strcmp(s, list) != 0){
        printf("Error in suffix list %s %s\n", word, s);
    }
    free_(s);
    free_fsm_parse_list(parse_list);
}

void test_suffix_list(Fsm_morphological_analyzer_ptr fsm) {
    test_suffix_list_single(fsm, 0, "açılır", "VerbalRoot(F5PR)(aç)+PassiveHl(açıl)+OtherTense2(açılır)");
    test_suffix_list_single(fsm, 0, "koparılarak", "VerbalRoot(F1P1)(kop)+CausativeAr(kopar)+PassiveHl(koparıl)+Adverb1(koparılarak)");
    test_suffix_list_single(fsm, 0, "toplama", "NominalRoot(topla)+Possessive(toplam)+Case1(toplama)");
    test_suffix_list_single(fsm, 0, "değerlendirmede", "NominalRoot(değer)+VerbalRoot(F5PR)(değerlen)+CausativeDHr(değerlendir)+NominalRoot(değerlendirme)+Case1(değerlendirmede)");
    test_suffix_list_single(fsm, 0, "soruşturmasının", "VerbalRoot(F5PR)(sor)+Reciprocal(soruş)+CausativeDHr(soruştur)+NominalRoot(soruşturma)+Possessive3(soruşturması)+Case1(soruşturmasının)");
    test_suffix_list_single(fsm, 0, "esaslarını", "AdjectiveRoot(esas)+Plural(esaslar)+Possessive(esasların)+AccusativeNoun(esaslarını)");
    test_suffix_list_single(fsm, 0, "güçleriyle", "AdjectiveRoot(güç)+Possesive3(güçleri)+Case1(güçleriyle)");
    test_suffix_list_single(fsm, 0, "bulmayacakları", "VerbalRoot(F5PW)(bul)+Negativema(bulma)+AdjectiveParticiple(bulmayacak)+Adjective(bulmayacakları)");
}

int main(){
    Fsm_morphological_analyzer_ptr fsm = create_fsm_morphological_analyzer3();
    test_get_last_lemma(fsm);
    test_get_last_lemma_with_tag(fsm);
    test_get_transition_list(fsm);
    test_with_list(fsm);
    test_suffix_list(fsm);
    free_fsm_morphological_analyzer(fsm);
}