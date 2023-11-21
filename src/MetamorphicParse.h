//
// Created by Olcay Taner YILDIZ on 15.10.2023.
//

#ifndef MORPHOLOGICALANALYSIS_METAMORPHICPARSE_H
#define MORPHOLOGICALANALYSIS_METAMORPHICPARSE_H

#include <ArrayList.h>
#include <Dictionary/Word.h>
#include "MorphologicalTag.h"

static char* meta_morphemes[109] = {"Ar", "Ar", "CA", "CA",
                                    "CA", "cAsHnA", "CH", "CHk",
                                    "DA", "DAn", "DH", "DHk",
                                    "DHkCA", "DHr", "DHr", "DHr",
                                    "H", "Hl", "Hm", "Hn",
                                    "Hn", "Hn", "HmHz", "HncH",
                                    "HnHz", "Hr", "Hr", "Hs",
                                    "Ht", "Hyor", "Hz", "k",
                                    "ki", "kü", "lAn", "lAr",
                                    "lArDHr", "lArH", "lArH'", "lAs",
                                    "lH", "lHk", "lHm", "m",
                                    "mA", "mA", "mAcA", "mAdAn",
                                    "mAk", "mAksHzHn", "mAktA", "mAlH",
                                    "mAzlHk", "mHs", "n", "n",
                                    "nA", "ncA", "nDA", "nDAn",
                                    "nH", "nHn", "nHz", "nlAr",
                                    "SA", "SAl", "sH", "SH",
                                    "SH", "SHn", "SHnHz", "SHnlAr",
                                    "SHz", "ŞAr", "t", "yA",
                                    "yA", "yAbil", "yAcAk", "yAcAk",
                                    "yAdur", "yAgel", "yAlH", "yAmA",
                                    "yAmAdAn", "yAn", "yArAk", "yAsH",
                                    "yDH", "yH", "yHcH", "yHm",
                                    "yHn", "yHncA", "yHp", "yHs",
                                    "yHver", "yHz", "yken", "ylA",
                                    "ymHs", "ysA", "z", "zsHn",
                                    "zsHnHz", "zlAr", "yAkal", "yAkoy",
                                    "yAgor"};

static Morphological_tag morphotactic_tags[109] = {
/**
 * Aorist Tense : Her hafta sonunda futbol oynarlar.
*/
        AORIST,
/**
 * Causative Form : Pişirmek
*/
        CAUSATIVE,
/**
 * As if : Yaşarmışcasına
*/
        ASIF,
/**
 * -LY : Akıllıca
*/
        LY,
/**
 * Equative : Öylece
*/
        EQUATIVE,
/**
 * As if
*/
        ASIF,
/**
 * Agent : Toplayıcı
*/
        AGENT,
/**
 * Dimension : Küçücük
*/
        DIMENSION,
/**
 * Locative : Aşağıda
*/
        LOCATIVE,
/**
 * Ablative : Okuldan
*/
        ABLATIVE,
/**
 * Past tense : Yaşandı
*/
        PASTTENSE,
/**
 * Past participle : Kırılmış
*/
        PASTPARTICIPLE,
/**
 * As long as : Yaşadıkça
*/
        ASLONGAS,
/**
 * Copula : Mavidir
*/
        COPULA,
/**
 * Since : Yıllardır
*/
        SINCE,
/**
 * Causitive
*/
        CAUSATIVE,
/**
 * 3rd person singular possessive : Onun
*/
        P3SG,
/**
 * Passive : Açıldı
*/
        PASSIVE,
/**
 * 1st person singular possessive : Benim
*/
        P1SG,
/**
 * Reflexive : Kendi
*/
        REFLEXIVE,
/**
 * Passive
*/
        PASSIVE,
/**
 * 2nd person singular possessive :Senin
*/
        P2SG,
/**
 * 1st person plural possessive :  Bizim
*/
        P1PL,
/**
 * Ordinal Number : Altıncı
*/
        ORDINAL,
/**
 * 2nd person plural possessive : Sizin
*/
        P2PL,
/**
 * Aorist
*/
        AORIST,
/**
 * Causitive
*/
        CAUSATIVE,
/**
 * Reciprocal verb : Görüşmek
*/
        RECIPROCAL,
/**
 * Causitive
*/
        CAUSATIVE,
/**
 * Progressive1 : Görüyor
*/
        PROGRESSIVE1,
/**
 * 1st person plural agreement : Biz gideriz
*/
        A1PL,
/**
 * 1st person plural agreement
*/
        A1PL,
/**
 * Relative : Gelenin
*/
        RELATIVE,
/**
 * Relative
*/
        RELATIVE,
/**
 * Acquire : Kazanılan
*/
        ACQUIRE,
/**
 * 3rd person plural agreement : Onlar giderler
*/
        A3PL,
/**
 * Since
*/
        SINCE,
/**
 * 3rd person plural possessive : Onların
*/
        P3PL,
/**
 * 3rd person plural possessive
*/
        P3PL,
/**
 * Become : Abideleş
*/
        BECOME,
/**
 * With : Kalemle
*/
        WITH,
/**
 * Ness : Ağırbaşlılık
*/
        NESS,
/**
 * 1st person plural agreement
*/
        A1PL,
/**
 * 1st person singular agreement : Ben giderim
*/
        A1SG,
/**
 * Infinitive2 : Yapma
*/
        INFINITIVE2,
/**
 * Negative : Yapama
*/
        NEGATIVE,
/**
 * Act of : Aldatmaca
*/
        ACTOF,
/**
 * Without having done so : Çaktırmadan
*/
        WITHOUTHAVINGDONESO,
/**
 * Infinitive : Yapmak
*/
        INFINITIVE,
/**
 * Without having done so
*/
        WITHOUTHAVINGDONESO,
/**
 * Progressive2 : Görmekte
*/
        PROGRESSIVE2,
/**
 * Necessity : Yapmalı
*/
        NECESSITY,
/**
 * Notable fsm_state : Anlaşılmazlık
*/
        NOTABLESTATE,
/**
 * Narrative Narrative Past Tense : Oluşmuş
*/
        NARRATIVE,
/**
 * 2nd person singuular agreement : Sen gelirsin
*/
        A2SG,
/**
 * Passive
*/
        PASSIVE,
/**
 * Dative case : Bana
*/
        DATIVE,
/**
 * Equative
*/
        EQUATIVE,
/**
 * Locative
*/
        LOCATIVE,
/**
 * Ablative
*/
        ABLATIVE,
/**
 * Accusatıve : Beni
*/
        ACCUSATIVE,
/**
 * Genitive : Benim
*/
        GENITIVE,
/**
 * 2nd person plural agreement : Siz gelirsiniz
*/
        A2PL,
/**
 * 3rd person plural agreement
*/
        A3PL,
/**
 * Desire/Past Auxiliary : Çıkarsa
*/
        DESIRE,
/**
 * Related to : Davranışsal
*/
        RELATED,
/**
 * 3rd person singular possessive
*/
        P3SG,
/**
 * Just like : Destansı
*/
        JUSTLIKE,
/**
 * Almost : Dikensi
*/
        ALMOST,
/**
 * 2nd person singuular agreement
*/
        A2SG,
/**
 * 2nd person plural agreement
*/
        A2PL,
/**
 * 3rd person plural agreement
*/
        A3PL,
/**
 * Without : Dikişsiz
*/
        WITHOUT,
/**
 * Distributive : altışar
*/
        DISTRIBUTIVE,
/**
 * Causitive form
*/
        CAUSATIVE,
/**
 * Dative case
*/
        DATIVE,
/**
 * Optative : Doğanaya
*/
        OPTATIVE,
/**
 * Ability, possibility : Olabilir
*/
        ABLE,
/**
 * Future participle : Gülecek
*/
        FUTUREPARTICIPLE,
/**
 * Future : Yağacak
*/
        FUTURE,
/**
 * Continuous : Yapadur
*/
        REPEAT,
/**
 * Ever since : Çıkagel
*/
        EVERSINCE,
/**
 * Since doing so : Amasyalı
*/
        SINCEDOINGSO,
/**
 * Not able fsm_state : Anlaşılmazlık
*/
        NOTABLESTATE,
/**
 * Wıthout beıng able to have done so: kararlamadan
*/
        WITHOUTBEINGABLETOHAVEDONESO,
/**
 * Present partıcıple : Sarılan
*/
        PRESENTPARTICIPLE,
/**
 * By doing so : Zıplayarak
*/
        BYDOINGSO,
/**
 * Projection : Öpülesi
*/
        FEELLIKE,
/**
 * Past tense : Gitti
*/
        PASTTENSE,
/**
 * Accusative
*/
        ACCUSATIVE,
/**
 * Agent
*/
        AGENT,
/**
 * 1st person singuular agreement
*/
        A1SG,
/**
 * 2nd person plural agreement
*/
        A2PL,
/**
 * When : Okuyunca
*/
        WHEN,
/**
 * After doing so : Yapıp
*/
        AFTERDOINGSO,
/**
 * Infinitive : Yapış
*/
        INFINITIVE3,
/**
 * Hastility : Yapıver
*/
        HASTILY,
/**
 * 1st person plural agreement
*/
        A1PL,
/**
 * While : Gelirken
*/
        WHILE,
/**
 * Instrumental : Kışın, arabayla
*/
        INSTRUMENTAL,
/**
 * Narrative
*/
        NARRATIVE,
/**
 * Conditional : Gelirse
*/
        CONDITIONAL,
/**
 * 3rd person singuular agreement : O gelir
*/
        A3SG,
/**
 * 2nd person singuular agreement
*/
        A2SG,
/**
 * 2nd person plural agreement
*/
        A2PL,
/**
 * 3rd person plural agreement
*/
        A3PL,
/**
 * Stay : Bakakal
*/
        STAY,
/**
 * Start : Alıkoy
*/
        START,
/**
 * Repeat : Yapagör
*/
        REPEAT};

struct metamorphic_parse{
    Array_list_ptr meta_morpheme_list;
    char* root;
};

typedef struct metamorphic_parse Metamorphic_parse;

typedef Metamorphic_parse *Metamorphic_parse_ptr;

Metamorphic_parse_ptr create_metamorphic_parse(const char* parse);

void free_metamorphic_parse(Metamorphic_parse_ptr metamorphic_parse);

Array_list_ptr get_meta_morpheme_tag1(char* tag);

void add_meta_morpheme_list(Metamorphic_parse_ptr metamorphic_parse, const char* new_tactic_set);

void remove_meta_morpheme_from_index(Metamorphic_parse_ptr metamorphic_parse, int index);

char* get_meta_morpheme(const Metamorphic_parse* metamorphic_parse, int index);

char* metamorphic_parse_to_string(const Metamorphic_parse* metamorphic_parse);

#endif //MORPHOLOGICALANALYSIS_METAMORPHICPARSE_H
