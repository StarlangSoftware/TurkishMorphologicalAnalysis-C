//
// Created by Olcay Taner YILDIZ on 24.10.2023.
//

#ifndef MORPHOLOGICALANALYSIS_FSMPARSELIST_H
#define MORPHOLOGICALANALYSIS_FSMPARSELIST_H

#include <ArrayList.h>
#include <Dictionary/TxtWord.h>
#include "FsmParse.h"

static char* longestRootExceptions[] = {"acağı acak NOUN VERB", "acağım acak NOUN VERB", "acağımı acak NOUN VERB", "acağımız acak NOUN VERB", "acağın acak NOUN VERB",
                                        "acağına acak NOUN VERB", "acağını acak NOUN VERB", "acağının acak NOUN VERB", "acağınız acak NOUN VERB", "acağınıza acak NOUN VERB",
                                        "acağınızdır acak NOUN VERB", "acağınızı acak NOUN VERB", "acağınızın acak NOUN VERB", "acağız acak NOUN VERB", "acakları acak NOUN VERB",
                                        "acaklarını acak NOUN VERB", "acaksa acak NOUN VERB", "acaktır acak NOUN VERB", "ardım ar NOUN VERB", "arız ar NOUN VERB",
                                        "arken ar NOUN VERB", "arsa ar NOUN VERB", "arsak ar NOUN VERB", "arsanız ar NOUN VERB", "arsınız ar NOUN VERB",
                                        "eceği ecek NOUN VERB", "eceğim ecek NOUN VERB", "eceğimi ecek NOUN VERB", "eceğimiz ecek NOUN VERB", "eceğin ecek NOUN VERB",
                                        "eceğine ecek NOUN VERB", "eceğini ecek NOUN VERB", "eceğinin ecek NOUN VERB", "eceğiniz ecek NOUN VERB", "eceğinizdir ecek NOUN VERB",
                                        "eceğinize ecek NOUN VERB", "eceğinizi ecek NOUN VERB", "eceğinizin ecek NOUN VERB", "eceğiz ecek NOUN VERB", "ecekleri ecek NOUN VERB",
                                        "eceklerini ecek NOUN VERB", "ecekse ecek NOUN VERB", "ecektir ecek NOUN VERB", "erdim er NOUN VERB", "eriz er NOUN VERB",
                                        "erken er NOUN VERB", "erse er NOUN VERB", "ersek er NOUN VERB", "erseniz er NOUN VERB", "ersiniz er NOUN VERB",
                                        "ilen i VERB VERB", "ilene i VERB VERB", "ilin i VERB VERB", "ilince i VERB VERB", "imiz i ADJ NOUN",
                                        "in i ADJ NOUN", "inde i ADJ NOUN", "ine i ADJ NOUN", "ini i ADJ NOUN", "inin i ADJ NOUN",
                                        "ılan ı NOUN VERB", "ılana ı NOUN VERB", "ılın ı NOUN VERB", "ılınca ı NOUN VERB", "la la VERB NOUN",
                                        "lar la VERB NOUN", "lardan la VERB NOUN", "lardandır la VERB NOUN", "lardır la VERB NOUN", "ları la VERB NOUN",
                                        "larıdır la VERB NOUN", "larım la VERB NOUN", "larımda la VERB NOUN", "larımdan la VERB NOUN", "larımız la VERB NOUN", "larımıza la VERB NOUN",
                                        "larımızda la VERB NOUN", "larımızdan la VERB NOUN", "larımızdaydı la VERB NOUN", "larımızı la VERB NOUN", "larımızın la VERB NOUN",
                                        "larımızla la VERB NOUN", "ların la VERB NOUN", "larına la VERB NOUN", "larında la VERB NOUN", "larındaki la VERB NOUN",
                                        "larındakiler la VERB NOUN", "larındakilere la VERB NOUN", "larındakileri la VERB NOUN", "larındakilerin la VERB NOUN", "larından la VERB NOUN",
                                        "larındandır la VERB NOUN", "larındaysa la VERB NOUN", "larını la VERB NOUN", "larının la VERB NOUN", "larınız la VERB NOUN",
                                        "larınıza la VERB NOUN", "larınızda la VERB NOUN", "larınızdaki la VERB NOUN", "larınızdan la VERB NOUN", "larınızı la VERB NOUN",
                                        "larınızın la VERB NOUN", "larınızla la VERB NOUN", "larıyla la VERB NOUN", "le le VERB NOUN", "ler le VERB NOUN",
                                        "lerden le VERB NOUN", "lerdendir le VERB NOUN", "lerdir le VERB NOUN", "leri le VERB NOUN", "leridir le VERB NOUN",
                                        "lerim le VERB NOUN", "lerimden le VERB NOUN", "lerimiz le VERB NOUN", "lerimizde le VERB NOUN", "lerimizden le VERB NOUN",
                                        "lerimizdeydi le VERB NOUN", "lerimize le VERB NOUN", "lerimizi le VERB NOUN", "lerimizin le VERB NOUN", "lerimizle le VERB NOUN",
                                        "lerin le VERB NOUN", "lerinde le VERB NOUN", "lerindeki le VERB NOUN", "lerindekiler le VERB NOUN", "lerindekilere le VERB NOUN",
                                        "lerindekileri le VERB NOUN", "lerindekilerin le VERB NOUN", "lerinden le VERB NOUN", "lerindendir le VERB NOUN", "lerindeyse le VERB NOUN",
                                        "lerine le VERB NOUN", "lerini le VERB NOUN", "lerinin le VERB NOUN", "leriniz le VERB NOUN", "lerinizde le VERB NOUN",
                                        "lerinizdeki le VERB NOUN", "lerinizden le VERB NOUN", "lerinize le VERB NOUN", "lerinizi le VERB NOUN", "lerinizin le VERB NOUN",
                                        "lerinizle le VERB NOUN", "leriyle le VERB NOUN", "madan ma NOUN VERB", "malı ma NOUN VERB",
                                        "malıdır ma NOUN VERB", "malıdırlar ma NOUN VERB", "malılar ma NOUN VERB", "malısınız ma NOUN VERB", "malıyım ma NOUN VERB",
                                        "malıyız ma NOUN VERB", "mam ma NOUN VERB", "mama ma NOUN VERB", "mamız ma NOUN VERB", "mamıza ma NOUN VERB",
                                        "mamızı ma NOUN VERB", "manız ma NOUN VERB", "manızda ma NOUN VERB", "manızdır ma NOUN VERB", "manızı ma NOUN VERB",
                                        "manızla ma NOUN VERB", "ması ma NOUN VERB", "masıdır ma NOUN VERB", "masın ma NOUN VERB", "masına ma NOUN VERB",
                                        "masında ma NOUN VERB", "masındaki ma NOUN VERB", "masını ma NOUN VERB", "masıyla ma NOUN VERB", "mdan m NOUN NOUN",
                                        "meden me NOUN VERB", "meli me NOUN VERB", "melidir me NOUN VERB", "melidirler me NOUN VERB", "meliler me NOUN VERB",
                                        "melisiniz me NOUN VERB", "meliyim me NOUN VERB", "meliyiz me NOUN VERB", "mem me NOUN VERB", "meme me NOUN VERB",
                                        "memiz me NOUN VERB", "memize me NOUN VERB", "memizi me NOUN VERB", "meniz me NOUN VERB", "menizde me NOUN VERB",
                                        "menizdir me NOUN VERB", "menizi me NOUN VERB", "menizle me NOUN VERB", "mesi me NOUN VERB", "mesidir me NOUN VERB",
                                        "mesin me NOUN VERB", "mesinde me NOUN VERB", "mesindeki me NOUN VERB", "mesine me NOUN VERB", "mesini me NOUN VERB",
                                        "mesiyle me NOUN VERB", "mişse miş NOUN VERB", "mını m NOUN NOUN", "mışsa mış NOUN VERB", "mız m NOUN NOUN",
                                        "na n NOUN NOUN", "ne n NOUN NOUN", "nin n NOUN NOUN", "niz n NOUN NOUN",
                                        "nın n NOUN NOUN", "nız n NOUN NOUN", "rdim r NOUN VERB", "rdım r NOUN VERB", "riz r NOUN VERB",
                                        "rız r NOUN VERB", "rken r NOUN VERB", "rken r NOUN VERB", "rsa r NOUN VERB", "rsak r NOUN VERB",
                                        "rsanız r NOUN VERB", "rse r NOUN VERB", "rsek r NOUN VERB", "rseniz r NOUN VERB", "rsiniz r NOUN VERB",
                                        "rsınız r NOUN VERB", "sa sa VERB ADJ", "se se VERB ADJ", "ulan u NOUN VERB", "un un VERB NOUN",
                                        "üne ün VERB NOUN", "unun un VERB NOUN", "ince i NOUN VERB", "unca u NOUN VERB", "ınca ı NOUN VERB",
                                        "unca un NOUN VERB", "ilen ile VERB VERB"};
struct fsm_parse_list{
    Array_list_ptr fsm_parses;
};

typedef struct fsm_parse_list Fsm_parse_list;

typedef Fsm_parse_list *Fsm_parse_list_ptr;

Fsm_parse_list_ptr create_fsm_parse_list(Array_list_ptr fsmParses);

Fsm_parse_list_ptr create_fsm_parse_list2();

void free_fsm_parse_list(Fsm_parse_list_ptr fsm_parse_list);

char* root_words(Fsm_parse_list_ptr fsm_parse_list);

void reduce_to_parses_with_same_root_and_pos(Fsm_parse_list_ptr fsm_parse_list, Txt_word_ptr current_with_pos);

Fsm_parse_ptr get_parse_with_longest_root_word(Fsm_parse_list_ptr fsm_parse_list);

bool is_longest_root_exception(Fsm_parse_list_ptr fsm_parse_list, Fsm_parse_ptr fsm_parse);

void reduce_to_parses_with_same_root(Fsm_parse_list_ptr fsm_parse_list, char* currentRoot);

Array_list_ptr construct_parse_list_for_different_root_with_pos(Fsm_parse_list_ptr fsm_parse_list);

char* fsm_parse_list_to_string(Fsm_parse_list_ptr fsm_parse_list);

char* parses_without_prefix_and_suffix(Fsm_parse_list_ptr fsm_parse_list);

Fsm_parse_ptr get_fsm_parse(Fsm_parse_list_ptr fsm_parse_list, int index);

Fsm_parse_list_ptr clone_fsm_parse_list(Fsm_parse_list_ptr fsm_parse_list);

#endif //MORPHOLOGICALANALYSIS_FSMPARSELIST_H
