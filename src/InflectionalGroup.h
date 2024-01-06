//
// Created by Olcay Taner YILDIZ on 14.10.2023.
//

#ifndef MORPHOLOGICALANALYSIS_INFLECTIONALGROUP_H
#define MORPHOLOGICALANALYSIS_INFLECTIONALGROUP_H

#include <ArrayList.h>
#include "MorphologicalTag.h"

static char* tags[131] = {"Noun", "Adv", "Adj", "Verb", "A1sg",
                          "A2sg", "A3sg", "A1Pl", "A2pl", "A3pl",
                          "P1sg", "P2sg", "P3sg", "P1Pl", "P2pl",
                          "P3pl", "Prop", "Pnon", "Nom", "With",
                          "Without", "Acc", "Dat", "Gen", "Abl",
                          "Zero", "Able", "Neg", "Past",
                          "Conj", "Det", "Dup", "Interj", "Num",
                          "PostP", "Punc", "Ques", "Agt", "ByDoingSo",
                          "Card", "Caus", "DemonsP", "Distrib", "FitFor",
                          "FutPart", "Inf", "Ness", "Ord", "Pass",
                          "PastPart", "PresPart", "QuesP", "QuantP", "Range",
                          "Ratio", "Real", "Recip", "Reflex", "ReflexP",
                          "Time", "When", "While", "WithoutHavingDoneSo", "PCAbl",
                          "PCAcc", "PCDat", "PCGen", "PCIns", "PCNom",
                          "Acquire", "ActOf", "AfterDoingSo", "Almost", "As",
                          "AsIf", "Become", "EverSince", "FeelLike", "Hastily",
                          "InBetween", "JustLike", "Ly", "NotAbleState", "Related",
                          "Repeat", "Since", "SinceDoingSo", "Start", "Stay",
                          "Equ", "Ins", "Aor", "Desr", "Fut",
                          "Imp", "Narr", "Neces", "Opt", "Past",
                          "Pres", "Prog1", "Prog2", "Cond", "Cop",
                          "Pos", "Pron", "Loc", "Rel", "Demons",
                          "Inf2", "Inf3", "BSTag", "ESTag", "BTTag",
                          "ETTag", "BDTag", "EDTag", "Inf1", "AsLongAs",
                          "Dist", "Adamantly", "Percent", "WithoutBeingAbleToHaveDoneSo", "Dim",
                          "Pers", "Fraction", "Hashtag", "Email", "Date", "Code", "Metric"};

static Morphological_tag morpho_tags[131] = {NOUN, ADVERB, ADJECTIVE,
                                             VERB, A1SG, A2SG, A3SG, A1PL,
                                             A2PL, A3PL, P1SG, P2SG, P3SG, P1PL,
                                             P2PL, P3PL, PROPERNOUN, PNON, NOMINATIVE,
                                             WITH, WITHOUT, ACCUSATIVE, DATIVE, GENITIVE,
                                             ABLATIVE, ZERO, ABLE, NEGATIVE, PASTTENSE,
                                             CONJUNCTION, DETERMINER, DUPLICATION, INTERJECTION, NUMBER,
                                             POSTPOSITION, PUNCTUATION, QUESTION, AGENT, BYDOINGSO,
                                             CARDINAL, CAUSATIVE, DEMONSTRATIVEPRONOUN, DISTRIBUTIVE, FITFOR,
                                             FUTUREPARTICIPLE, INFINITIVE, NESS, ORDINAL, PASSIVE,
                                             PASTPARTICIPLE, PRESENTPARTICIPLE, QUESTIONPRONOUN, QUANTITATIVEPRONOUN, RANGE,
                                             RATIO, REAL, RECIPROCAL, REFLEXIVE, REFLEXIVEPRONOUN,
                                             TIME, WHEN, WHILE, WITHOUTHAVINGDONESO, PCABLATIVE,
                                             PCACCUSATIVE, PCDATIVE, PCGENITIVE, PCINSTRUMENTAL, PCNOMINATIVE,
                                             ACQUIRE, ACTOF, AFTERDOINGSO, ALMOST, AS,
                                             ASIF, BECOME, EVERSINCE, FEELLIKE, HASTILY,
                                             INBETWEEN, JUSTLIKE, LY, NOTABLESTATE, RELATED,
                                             REPEAT, SINCE, SINCEDOINGSO, START, STAY,
                                             EQUATIVE, INSTRUMENTAL, AORIST, DESIRE, FUTURE,
                                             IMPERATIVE, NARRATIVE, NECESSITY, OPTATIVE, PAST,
                                             PRESENT, PROGRESSIVE1, PROGRESSIVE2, CONDITIONAL, COPULA,
                                             POSITIVE, PRONOUN, LOCATIVE, RELATIVE, DEMONSTRATIVE,
                                             INFINITIVE2, INFINITIVE3, BEGINNINGOFSENTENCE, ENDOFSENTENCE, BEGINNINGOFTITLE,
                                             ENDOFTITLE, BEGINNINGOFDOCUMENT, ENDOFDOCUMENT, INFINITIVE, ASLONGAS,
                                             DISTRIBUTIVE, ADAMANTLY, PERCENT, WITHOUTBEINGABLETOHAVEDONESO, DIMENSION,
                                             PERSONALPRONOUN, FRACTION, HASHTAG, EMAIL, DATE,
                                             CODE, METRIC};

struct inflectional_group{
    Array_list_ptr IG;
};

typedef struct inflectional_group Inflectional_group;

typedef Inflectional_group *Inflectional_group_ptr;

Inflectional_group_ptr create_inflectional_group(const char* IG);

void free_inflectional_group(Inflectional_group_ptr inflectional_group);

Morphological_tag get_morphological_tag(const char* tag);

char* get_tag(Morphological_tag tag);

Morphological_tag get_tag_with_index(const Inflectional_group* inflectional_group, int index);

Morphological_tag contains_case(const Inflectional_group* inflectional_group);

bool contains_plural(const Inflectional_group* inflectional_group);

bool contains_tag(const Inflectional_group* inflectional_group, Morphological_tag tag);

bool contains_possessive(const Inflectional_group* inflectional_group);

char* inflectional_group_to_string(const Inflectional_group* inflectional_group);

Inflectional_group_ptr clone_inflectional_group(const Inflectional_group* inflectional_group);

#endif //MORPHOLOGICALANALYSIS_INFLECTIONALGROUP_H
