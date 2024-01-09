//
// Created by Olcay Taner YILDIZ on 5.11.2023.
//

#include <Memory/Memory.h>
#include "../src/FsmMorphologicalAnalyzer.h"

void test_transition(Fsm_morphological_analyzer_ptr fsm, char* list[], int size){
    for (int i = 0; i < size; i++){
        Fsm_parse_list_ptr parse_list = morphological_analysis(fsm, list[i]);
        if (parse_list->fsm_parses->size == 0){
            printf("Error in word %s\n", list[i]);
        }
        free_fsm_parse_list(parse_list);
    }
}

int main(){
    start_large_memory_check();
    Fsm_morphological_analyzer_ptr fsm = create_fsm_morphological_analyzer3();
    test_transition(fsm, (char*[]){"3'tü"}, 1);
    test_transition(fsm, (char*[]){"3'tü", "1'di", "2'ydi", "4'tü", "5'ti", "6'ydı", "7'ydi", "8'di", "9'du", "30'du", "40'tı", "60'tı", "70'ti", "50'ydi"}, 14);
    test_transition(fsm, (char*[]){"alkole", "anormale", "sakala", "kabala", "faika", "halika", "kediye", "eve"}, 8);
    test_transition(fsm, (char*[]){"2'yi", "5'i", "9'u", "10'u", "30'u", "3'ü", "4'ü", "100'ü", "6'yı", "40'ı", "60'ı", "90'ı"}, 12);
    test_transition(fsm, (char*[]){"6'ya", "9'a", "10'a", "30'a", "40'a", "60'a", "90'a", "8'e", "5'e", "4'e", "1'e", "3'e", "7'ye"}, 13);
    test_transition(fsm, (char*[]){"büyülüyor", "bölümlüyor", "buğuluyor", "bulguluyor", "açıklıyor", "çalkalıyor"}, 6);
    test_transition(fsm, (char*[]){"gripçi", "güllaççı", "gülütçü", "gülükçü"}, 4);
    test_transition(fsm, (char*[]){"altışar", "yedişer", "üçer", "beşer", "dörder"}, 5);
    test_transition(fsm, (char*[]){"koştu", "kitaptı", "kaçtı", "evdi", "fraktı", "sattı", "aftı", "kesti", "ahtı"}, 9);
    test_transition(fsm, (char*[]){"yaparmışçasına", "yiyip", "sana", "bununla", "onunla", "şununla", "bana"}, 7);
    test_transition(fsm, (char*[]){"diyor", "yiyor"}, 2);
    test_transition(fsm, (char*[]){"buyruldu", "yoğruldu"}, 2);
    test_transition(fsm, (char*[]){"karasuyu", "suyu"}, 2);
    test_transition(fsm, (char*[]){"zıddı", "serhaddi", "tıbbı", "ceddi", "fenni", "haddi", "hazzı", "şakkı", "şakı", "halli", "hali"}, 11);
    test_transition(fsm, (char*[]){"hizbi", "kaybı", "ahdi", "nesci", "zehri", "zikri", "metni", "metini", "katli", "katili"}, 10);
    test_transition(fsm, (char*[]){"adabı", "amibi", "armudu", "ağacı", "akacı", "arkeoloğu", "filoloğu", "ahengi", "küngü", "kitaplığı", "küllüğü", "adedi", "adeti", "ağıdı", "ağıtı", "anotu", "anodu", "Kuzguncuk'u", "Leylak'ı"}, 19);
    test_transition(fsm, (char*[]){"cezbediyor", "ediyor", "bahsediyor"}, 3);
    free_fsm_morphological_analyzer(fsm);
    end_memory_check();
}