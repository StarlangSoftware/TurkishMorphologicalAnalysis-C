//
// Created by Olcay Taner YILDIZ on 23.10.2023.
//

#include <stdlib.h>
#include <Dictionary/TxtWord.h>
#include <stdio.h>
#include <string.h>
#include <FileUtils.h>
#include "FsmParse.h"
#include "InflectionalGroup.h"
#include "Transition.h"

/**
 * A constructor of FsmParse class which takes a Word as an input and assigns it to root variable.
 *
 * @param root Word input.
 */
Fsm_parse_ptr create_fsm_parse(Txt_word_ptr root) {
    Fsm_parse_ptr result = create_fsm_parse2();
    result->root = root;
    return result;
}

Fsm_parse_ptr create_fsm_parse2() {
    Fsm_parse_ptr result = malloc(sizeof(Fsm_parse));
    result->form_list = create_array_list();
    result->inflectional_groups = create_array_list();
    result->suffix_list = create_array_list();
    result->transition_list = create_array_list();
    result->with_list = create_array_list();
    result->form = NULL;
    result->initial_pos = NULL;
    result->pos = NULL;
    result->possessive_agreement = NULL;
    result->root = NULL;
    result->verb_agreement = NULL;
    return result;
}

void free_fsm_parse(Fsm_parse_ptr fsm_parse) {
    free(fsm_parse->form);
    free_array_list(fsm_parse->form_list, NULL);
    free_array_list(fsm_parse->inflectional_groups, NULL);
    free_array_list(fsm_parse->suffix_list, NULL);
    free_array_list(fsm_parse->transition_list, NULL);
    free_array_list(fsm_parse->with_list, NULL);
    free(fsm_parse);
}

void update_fsm_parse_with_state_and_name(Fsm_parse_ptr fsm_parse, char *name, Fsm_State_ptr start_state) {
    fsm_parse->root = create_txt_word(name);
    fsm_parse->form = str_copy(fsm_parse->form, fsm_parse->root->word->name);
    fsm_parse->pos = start_state->pos;
    fsm_parse->initial_pos = start_state->pos;
    array_list_add(fsm_parse->suffix_list, start_state);
    array_list_add(fsm_parse->form_list, fsm_parse->form);
}

/**
 * Another constructor of FsmParse class which takes an Integer number and a Fsm_State as inputs.
 * First, it creates a TxtWord with given number and adds flag to this number as IS_SAYI and initializes root variable with
 * number TxtWord. It also initializes form with root's name, pos and initialPos with given Fsm_State's POS, creates 4 new
 * ArrayList suffixList, formList, transitionList and withList and adds given Fsm_State to suffixList, form to
 * formList.
 *
 * @param number     Integer input.
 * @param startState Fsm_State input.
 */
Fsm_parse_ptr create_fsm_parse3(int number, Fsm_State_ptr start_state) {
    char name[MAX_WORD_LENGTH];
    Fsm_parse_ptr result = create_fsm_parse2();
    sprintf(name, "%d", number);
    update_fsm_parse_with_state_and_name(result, name, start_state);
    add_flag(result->root, "IS_SAYI");
    return result;
}

/**
 * Another constructor of FsmParse class which takes a Double number and a Fsm_State as inputs.
 * First, it creates a TxtWord with given number and adds flag to this number as IS_SAYI and initializes root variable with
 * number TxtWord. It also initializes form with root's name, pos and initialPos with given Fsm_State's POS, creates 4 new
 * ArrayList suffixList, formList, transitionList and withList and adds given Fsm_State to suffixList, form to
 * formList.
 *
 * @param number     Double input.
 * @param startState Fsm_State input.
 */
Fsm_parse_ptr create_fsm_parse4(double number, Fsm_State_ptr start_state) {
    char name[MAX_WORD_LENGTH];
    Fsm_parse_ptr result = create_fsm_parse2();
    sprintf(name, "%lf", number);
    update_fsm_parse_with_state_and_name(result, name, start_state);
    add_flag(result->root, "IS_SAYI");
    return result;
}

/**
 * Another constructor of FsmParse class which takes a String punctuation and a Fsm_State as inputs.
 * First, it creates a TxtWord with given punctuation and initializes root variable with this TxtWord.
 * It also initializes form with root's name, pos and initialPos with given Fsm_State's POS, creates 4 new
 * ArrayList suffixList, formList, transitionList and withList and adds given Fsm_State to suffixList, form to
 * formList.
 *
 * @param punctuation String input.
 * @param startState  Fsm_State input.
 */
Fsm_parse_ptr create_fsm_parse5(char *punctuation, Fsm_State_ptr start_state) {
    Fsm_parse_ptr result = create_fsm_parse2();
    update_fsm_parse_with_state_and_name(result, punctuation, start_state);
    return result;
}

/**
 * Another constructor of FsmParse class which takes a TxtWord root and a Fsm_State as inputs.
 * First, initializes root variable with this TxtWord. It also initializes form with root's name, pos and
 * initialPos with given Fsm_State's POS, creates 4 new ArrayList suffixList, formList, transitionList
 * and withList and adds given Fsm_State to suffixList, form to formList.
 *
 * @param root       TxtWord input.
 * @param startState Fsm_State input.
 */
Fsm_parse_ptr create_fsm_parse6(Txt_word_ptr root, Fsm_State_ptr start_state) {
    Fsm_parse_ptr result = create_fsm_parse2();
    result->root = root;
    result->form = str_copy(result->form, root->word->name);
    result->pos = start_state->pos;
    result->initial_pos = start_state->pos;
    array_list_add(result->suffix_list, start_state);
    array_list_add(result->form_list, result->form);
    return result;
}

/**
 * The constructInflectionalGroups method initially calls the transitionList method and assigns the resulting String
 * to the parse variable and creates a new ArrayList as iGs. If parse String contains a derivational boundary
 * it adds the substring starting from the 0 to the index of derivational boundary to the iGs. If it does not contain a DB,
 * it directly adds parse to the iGs. Then, creates and initializes new ArrayList as inflectionalGroups and fills with
 * the items of iGs.
 */
void construct_inflectional_groups(Fsm_parse_ptr fsm_parse) {
    char *parse = transition_list(fsm_parse);
    Array_list_ptr iGs = str_split2(parse, "^DB+");
    if (strcmp(((char *) array_list_get(iGs, 0)), "++Punc") == 0) {
        array_list_add(fsm_parse->inflectional_groups, create_inflectional_group("Punc"));
    } else {
        if (str_contains(array_list_get(iGs, 0), "+")) {
            String_ptr s2 = substring2(array_list_get(iGs, 0), str_find_c(array_list_get(iGs, 0), "+"));
            array_list_add(fsm_parse->inflectional_groups, create_inflectional_group(s2->s));
            free_string_ptr(s2);
        }
        for (int i = 1; i < iGs->size; i++) {
            array_list_add(fsm_parse->inflectional_groups,
                           create_inflectional_group(array_list_get(iGs, i)));
        }
    }
    free_array_list(iGs, free);
}

/**
 * The set_agreement method takes a String transitionName as an input and if it is one of the A1SG, A2SG, A3SG,
 * A1PL, A2PL or A3PL it assigns transitionName input to the verbAgreement variable. Or if it is ine of the PNON, P1SG, P2SG,P3SG,
 * P1PL, P2PL or P3PL it assigns transitionName input to the possesiveAgreement variable.
 *
 * @param transition_name String input.
 */
void set_agreement(Fsm_parse_ptr fsm_parse, char *transition_name) {
    if (transition_name != NULL &&
        string_in_list(transition_name, (char *[]) {"A1SG", "A2SG", "A3SG", "A1PL", "A2PL", "A3PL"}, 6)) {
        fsm_parse->verb_agreement = transition_name;
    }
    if (transition_name != NULL &&
        string_in_list(transition_name, (char *[]) {"PNON", "P1SG", "P2SG", "P3SG", "P1PL", "P2PL", "P3PL"}, 7)) {
        fsm_parse->possessive_agreement = transition_name;
    }
}

/**
 * The getLastLemmaWithTag method takes a String input _pos as an input. If given _pos is an initial _pos then it assigns
 * root to the lemma, and assign null otherwise.  Then, it loops i times where i ranges from 1 to size of the formList,
 * if the item at i-1 of transitionList is not null and contains a derivational boundary with _pos but not with ZERO,
 * it assigns the ith item of formList to lemma.
 *
 * @param _pos String input.
 * @return String output lemma.
 */
char *get_last_lemma_with_tag(Fsm_parse_ptr fsm_parse, char *_pos) {
    char *lemma;
    if (fsm_parse->initial_pos != NULL && strcmp(fsm_parse->initial_pos, _pos) == 0) {
        lemma = fsm_parse->root->word->name;
    } else {
        lemma = "";
    }
    for (int i = 1; i < fsm_parse->form_list->size; i++) {
        char word1[MAX_WORD_LENGTH], word2[MAX_WORD_LENGTH];
        sprintf(word1, "^DB+%s", _pos);
        sprintf(word2, "^DB+%s+ZERO", _pos);
        if (array_list_get(fsm_parse->transition_list, i - 1) != NULL &&
            str_contains(array_list_get(fsm_parse->transition_list, i - 1), word1) &&
            !str_contains(array_list_get(fsm_parse->transition_list, i - 1), word2)) {
            lemma = array_list_get(fsm_parse->form_list, i);
        }
    }
    return lemma;
}

/**
 * The getLastLemma method initially assigns root as lemma. Then, it loops i times where i ranges from 1 to size of the formList,
 * if the item at i-1 of transitionList is not null and contains a derivational boundary, it assigns the ith item of formList to lemma.
 *
 * @return String output lemma.
 */
char *get_last_lemma(Fsm_parse_ptr fsm_parse) {
    char *lemma = fsm_parse->root->word->name;
    for (int i = 1; i < fsm_parse->form_list->size; i++) {
        if (array_list_get(fsm_parse->transition_list, i - 1) != NULL &&
            str_contains(array_list_get(fsm_parse->transition_list, i - 1), "^DB+")) {
            lemma = array_list_get(fsm_parse->form_list, i);
        }
    }
    return lemma;
}

/**
 * The addSuffix method takes 5 different inputs; Fsm_State suffix, String _form, transition, with and toPos.
 * If the pos of given input suffix is not null, it then assigns it to the pos variable. If the pos of the given suffix
 * is null but given toPos is not null than it assigns toPos to pos variable. At the end, it adds suffix to the suffixList,
 * _form to the formList, transition to the transitionList and if given with is not 0, it is also added to withList.
 *
 * @param suffix     Fsm_State input.
 * @param _form       String input.
 * @param transition String input.
 * @param with       String input.
 * @param toPos      String input.
 */
void add_suffix(Fsm_parse_ptr fsm_parse,
                Fsm_State_ptr suffix,
                char *_form,
                char *transition,
                char *with,
                char *toPos) {
    if (suffix->pos != NULL) {
        fsm_parse->pos = suffix->pos;
    } else {
        if (toPos != NULL) {
            fsm_parse->pos = toPos;
        }
    }
    array_list_add(fsm_parse->suffix_list, suffix);
    fsm_parse->form = str_copy(fsm_parse->form, _form);
    array_list_add(fsm_parse->form_list, fsm_parse->form);
    array_list_add(fsm_parse->transition_list, transition);
    if (strcmp(with, "0") != 0) {
        array_list_add(fsm_parse->with_list, with);
    }
}

/**
 * The getStartState method returns the first item of suffixList ArrayList.
 *
 * @return the first item of suffixList ArrayList.
 */
Fsm_State_ptr get_start_state(Fsm_parse_ptr fsm_parse) {
    return array_list_get(fsm_parse->suffix_list, 0);
}

/**
 * The setForm method takes a String name as an input and assigns it to the form variable, then it removes the first item
 * of formList ArrayList and adds the given name to the formList.
 *
 * @param name String input to set form.
 */
void set_form(Fsm_parse_ptr fsm_parse, char *name) {
    fsm_parse->form = str_copy(fsm_parse->form, name);
    array_list_remove(fsm_parse->form_list, 0, NULL);
    array_list_add(fsm_parse->form_list, name);
}

/**
 * The getFinalSuffix method returns the last item of suffixList ArrayList.
 *
 * @return the last item of suffixList ArrayList.
 */
Fsm_State_ptr get_final_suffix(Fsm_parse_ptr fsm_parse) {
    return array_list_get(fsm_parse->suffix_list, fsm_parse->suffix_list->size - 1);
}

/**
 * The headerTransition method gets the first item of formList and checks for cases;
 * <p>
 * If it is &lt;DOC&gt;, it returns &lt;DOC&gt;+BDTAG which indicates the beginning of a document.
 * If it is &lt;/DOC&gt;, it returns &lt;/DOC&gt;+EDTAG which indicates the ending of a document.
 * If it is &lt;TITLE&gt;, it returns &lt;TITLE&gt;+BTTAG which indicates the beginning of a title.
 * If it is &lt;/TITLE&gt;, it returns &lt;/TITLE&gt;+ETTAG which indicates the ending of a title.
 * If it is &lt;S&gt;, it returns &lt;S&gt;+BSTAG which indicates the beginning of a sentence.
 * If it is &lt;/S&gt;, it returns &lt;/S&gt;+ESTAG which indicates the ending of a sentence.
 *
 * @return corresponding tags of the headers and an empty String if any case does not match.
 */
String_ptr header_transition(const Fsm_parse* fsm_parse) {
    char* tmp = array_list_get(fsm_parse->form_list, 0);
    if (strcmp(tmp, "<DOC>") == 0) {
        return create_string2("<DOC>+BDTAG");
    }
    if (strcmp(tmp, "</DOC>") == 0) {
        return create_string2("</DOC>+EDTAG");
    }
    if (strcmp(tmp, "<TITLE>") == 0) {
        return create_string2("<TITLE>+BTTAG");
    }
    if (strcmp(tmp, "</TITLE>") == 0) {
        return create_string2("</TITLE>+ETTAG");
    }
    if (strcmp(tmp, "<S>") == 0) {
        return create_string2("<S>+BSTAG");
    }
    if (strcmp(tmp, "</S>") == 0) {
        return create_string2("</S>+ESTAG");
    }
    return create_string();
}

/**
 * The pronounTransition method gets the first item of formList and checks for cases;
 * <p>
 * If it is "kendi", it returns kendi+PRON+REFLEXP which indicates a reflexive pronoun.
 * If it is one of the "hep, öbür, kimse, hiçbiri, tümü, çoğu, hepsi, biri, birbirleri, birbiri, bazı, kimi", it returns
 * +PRON+QUANTP which indicates a quantitative pronoun.
 * If it is one of the "o, bu, şu" and if it is "o" it also checks the first item of suffixList and if it is a PronounRoot(DEMONS),
 * it returns +PRON+DEMONSP which indicates a demonstrative pronoun.
 * If it is "ben", it returns +PRON+PERS+A1SG+PNON which indicates a 1st person singular agreement.
 * If it is "sen", it returns +PRON+PERS+A2SG+PNON which indicates a 2nd person singular agreement.
 * If it is "o" and the first item of suffixList, if it is a PronounRoot(PERS), it returns +PRON+PERS+A3SG+PNON which
 * indicates a 3rd person singular agreement.
 * If it is "biz", it returns +PRON+PERS+A1PL+PNON which indicates a 1st person plural agreement.
 * If it is "siz", it returns +PRON+PERS+A2PL+PNON which indicates a 2nd person plural agreement.
 * If it is "onlar" and the first item of suffixList, if it is a PronounRoot(PERS), it returns o+PRON+PERS+A3PL+PNON which
 * indicates a 3rd person plural agreement.
 * If it is one of the "nere, ne, kim, hangi", it returns +PRON+QUESP which indicates a question pronoun.
 *
 * @return corresponding transitions of pronouns and an empty String if any case does not match.
 */
String_ptr pronoun_transition(const Fsm_parse* fsm_parse) {
    char* form = array_list_get(fsm_parse->form_list, 0);
    Fsm_State_ptr suffix_state = array_list_get(fsm_parse->suffix_list, 0);
    if (strcmp(form, "kendi") == 0) {
        return create_string2("kendi+PRON+REFLEXP");
    }
    if (string_in_list(form, (char*[]){"hep", "öbür", "topu", "öteki", "kimse",
                                       "hiçbiri", "tümü", "çoğu", "hepsi", "herkes",
                                       "başkası", "birçoğu", "birçokları", "birbiri", "birbirleri",
                                       "biri", "birkaçı", "böylesi", "diğeri", "cümlesi",
                                       "bazı", "kimi"}, 22)){
        return create_string3(form, "+PRON+QUANTP");
    }
    if ((strcmp(form, "o") == 0 && strcmp(suffix_state->name, "PronounRoot(DEMONS)") == 0) || string_in_list(form, (char*[]) {"bu", "şu"}, 2)) {
        return create_string3(form, "+PRON+DEMONSP");
    }
    if (strcmp(form, "ben") == 0) {
        return create_string3(form, "+PRON+PERS+A1SG+PNON");
    }
    if (strcmp(form, "sen") == 0) {
        return create_string3(form, "+PRON+PERS+A2SG+PNON");
    }
    if (strcmp(form, "o") == 0 && strcmp(suffix_state->name, "PronounRoot(PERS)") == 0) {
        return create_string3(form, "+PRON+PERS+A3SG+PNON");
    }
    if (strcmp(form, "biz") == 0) {
        return create_string3(form, "+PRON+PERS+A1PL+PNON");
    }
    if (strcmp(form, "siz") == 0) {
        return create_string3(form, "+PRON+PERS+A2PL+PNON");
    }
    if (strcmp(form, "onlar") == 0) {
        return create_string2("o+PRON+PERS+A3PL+PNON");
    }
    if (string_in_list(form, (char*[]) {"nere", "ne", "kaçı", "kim", "hangi"}, 5)) {
        return create_string3(form, "+PRON+QUESP");
    }
    return create_string();
}

/**
 * The getTransitionList method first creates an empty String result, then gets the first item of suffixList and checks for cases;
 * <p>
 * If it is one of the "NominalRoot, NominalRootNoPossesive, CompoundNounRoot, NominalRootPlural", it assigns concatenation of first
 * item of formList and +NOUN to the result String.
 * Ex : Birincilik
 * <p>
 * If it is one of the "VerbalRoot, PassiveHn", it assigns concatenation of first item of formList and +VERB to the result String.
 * Ex : Başkalaştı
 * <p>
 * If it is "CardinalRoot", it assigns concatenation of first item of formList and +NUM+CARD to the result String.
 * Ex : Onuncu
 * <p>
 * If it is "FractionRoot", it assigns concatenation of first item of formList and NUM+FRACTION to the result String.
 * Ex : 1/2
 * <p>
 * If it is "TimeRoot", it assigns concatenation of first item of formList and +TIME to the result String.
 * Ex : 14:28
 * <p>
 * If it is "RealRoot", it assigns concatenation of first item of formList and +NUM+REAL to the result String.
 * Ex : 1.2
 * <p>
 * If it is "Punctuation", it assigns concatenation of first item of formList and +PUNC to the result String.
 * Ex : ,
 * <p>
 * If it is "Hashtag", it assigns concatenation of first item of formList and +HASHTAG to the result String.
 * Ex : #
 * <p>
 * If it is "DateRoot", it assigns concatenation of first item of formList and +DATE to the result String.
 * Ex : 11/06/2018
 * <p>
 * If it is "RangeRoot", it assigns concatenation of first item of formList and +RANGE to the result String.
 * Ex : 3-5
 * <p>
 * If it is "Email", it assigns concatenation of first item of formList and +EMAIL to the result String.
 * Ex : abc@
 * <p>
 * If it is "PercentRoot", it assigns concatenation of first item of formList and +PERCENT to the result String.
 * Ex : %12.5
 * <p>
 * If it is "DeterminerRoot", it assigns concatenation of first item of formList and +DET to the result String.
 * Ex : Birtakım
 * <p>
 * If it is "ConjunctionRoot", it assigns concatenation of first item of formList and +CONJ to the result String.
 * Ex : Ama
 * <p>
 * If it is "AdverbRoot", it assigns concatenation of first item of formList and +ADV to the result String.
 * Ex : Acilen
 * <p>
 * If it is "ProperRoot", it assigns concatenation of first item of formList and +NOUN+PROP to the result String.
 * Ex : Ahmet
 * <p>
 * If it is "HeaderRoot", it assigns the result of the headerTransition method to the result String.
 * Ex : &lt;DOC&gt;
 * <p>
 * If it is "InterjectionRoot", it assigns concatenation of first item of formList and +INTERJ to the result String.
 * Ex : Hey
 * <p>
 * If it is "DuplicateRoot", it assigns concatenation of first item of formList and +DUP to the result String.
 * Ex : Allak
 * <p>
 * If it is "CodeRoot", it assigns concatenation of first item of formList and +CODE to the result String.
 * Ex : 5000-WX
 * <p>
 * If it is "MetricRoot", it assigns concatenation of first item of formList and +METRIC to the result String.
 * Ex : 6cmx12cm
 * <p>
 * If it is "QuestionRoot", it assigns concatenation of first item of formList and +QUES to the result String.
 * Ex : Mı
 * <p>
 * If it is "PostP", and the first item of formList is one of the "karşı, ilişkin, göre, kadar, ait, yönelik, rağmen, değin,
 * dek, doğru, karşın, dair, atfen, binaen, hitaben, istinaden, mahsuben, mukabil, nazaran", it assigns concatenation of first
 * item of formList and +POSTP+PCDAT to the result String.
 * Ex : İlişkin
 * <p>
 * If it is "PostP", and the first item of formList is one of the "sonra, önce, beri, fazla, dolayı, itibaren, başka,
 * çok, evvel, ötürü, yana, öte, aşağı, yukarı, dışarı, az, gayrı", it assigns concatenation of first
 * item of formList and +POSTP+PCABL to the result String.
 * Ex : Başka
 * <p>
 * If it is "PostP", and the first item of formList is "yanısıra", it assigns concatenation of first
 * item of formList and +POSTP+PCGEN to the result String.
 * Ex : Yanısıra
 * <p>
 * If it is "PostP", and the first item of formList is one of the "birlikte, beraber", it assigns concatenation of first
 * item of formList and +PPOSTP+PCINS to the result String.
 * Ex : Birlikte
 * <p>
 * If it is "PostP", and the first item of formList is one of the "aşkın, takiben", it assigns concatenation of first
 * item of formList and +POSTP+PCACC to the result String.
 * Ex : Takiben
 * <p>
 * If it is "PostP", it assigns concatenation of first item of formList and +POSTP+PCNOM to the result String.
 * <p>
 * If it is "PronounRoot", it assigns result of the pronounTransition method to the result String.
 * Ex : Ben
 * <p>
 * If it is "OrdinalRoot", it assigns concatenation of first item of formList and +NUM+ORD to the result String.
 * Ex : Altıncı
 * <p>
 * If it starts with "Adjective", it assigns concatenation of first item of formList and +ADJ to the result String.
 * Ex : Güzel
 * <p>
 * At the end, it loops through the formList and concatenates each item with result String.
 *
 * @return String result accumulated with items of formList.
 */
char *transition_list(const Fsm_parse* fsm_parse) {
    String_ptr result;
    char* form = array_list_get(fsm_parse->form_list, 0);
    char* suffix = ((Fsm_State_ptr) array_list_get(fsm_parse->suffix_list, 0))->name;
    if (string_in_list(suffix, (char*[]) {"NominalRoot", "NominalRootNoPossesive", "CompoundNounRoot", "NominalRootPlural"}, 4)) {
        result = create_string3(form, "+NOUN");
    } else {
        if (starts_with(suffix, "VerbalRoot") || strcmp(suffix, "PassiveHn") == 0) {
            result = create_string3(form, "+VERB");
        } else {
            if (strcmp(suffix, "CardinalRoot") == 0) {
                result = create_string3(form, "+NUM+CARD");
            } else {
                if (strcmp(suffix, "FractionRoot") == 0) {
                    result = create_string3(form, "+NUM+FRACTION");
                } else {
                    if (strcmp(suffix, "TimeRoot") == 0) {
                        result = create_string3(form, "+TIME");
                    } else {
                        if (strcmp(suffix, "RealRoot") == 0) {
                            result = create_string3(form, "+NUM+REAL");
                        } else {
                            if (strcmp(suffix, "Punctuation") == 0) {
                                result = create_string3(form, "+PUNC");
                            } else {
                                if (strcmp(suffix, "Hashtag") == 0) {
                                    result = create_string3(form, "+HASHTAG");
                                } else {
                                    if (strcmp(suffix, "DateRoot") == 0) {
                                        result = create_string3(form, "+DATE");
                                    } else {
                                        if (strcmp(suffix, "RangeRoot") == 0) {
                                            result = create_string3(form, "+RANGE");
                                        } else {
                                            if (strcmp(suffix, "Email") == 0) {
                                                result = create_string3(form, "+EMAIL");
                                            } else {
                                                if (strcmp(suffix, "PercentRoot") == 0) {
                                                    result = create_string3(form, "+PERCENT");
                                                } else {
                                                    if (strcmp(suffix, "DeterminerRoot") == 0) {
                                                        result = create_string3(form, "+DET");
                                                    } else {
                                                        if (strcmp(suffix, "ConjunctionRoot") == 0) {
                                                            result = create_string3(form, "+CONJ");
                                                        } else {
                                                            if (strcmp(suffix, "AdverbRoot") == 0) {
                                                                result = create_string3(form, "+ADV");
                                                            } else {
                                                                if (strcmp(suffix, "ProperRoot") == 0) {
                                                                    result = create_string3(form, "+NOUN+PROP");
                                                                } else {
                                                                    if (strcmp(suffix, "HeaderRoot") == 0) {
                                                                        result = header_transition(fsm_parse);
                                                                    } else {
                                                                        if (strcmp(suffix, "InterjectionRoot") == 0){
                                                                            result = create_string3(form, "+INTERJ");
                                                                        } else {
                                                                            if (strcmp(suffix, "DuplicateRoot") == 0) {
                                                                                result = create_string3(form, "+DUP");
                                                                            } else {
                                                                                if (strcmp(suffix, "CodeRoot") == 0){
                                                                                    result = create_string3(form, "CODE");
                                                                                } else {
                                                                                    if (strcmp(suffix, "MetricRoot") == 0){
                                                                                        result = create_string3(form, "METRIC");
                                                                                    } else {
                                                                                        if (strcmp(suffix, "QuestionRoot") == 0) {
                                                                                            result = create_string2("mi+QUES");
                                                                                        } else {
                                                                                            if (strcmp(suffix, "PostP") == 0) {
                                                                                                if (string_in_list(form, (char*[]) {"karşı", "ilişkin", "göre", "kadar", "ait", "yönelik", "rağmen", "değin", "dek", "doğru", "karşın", "dair", "atfen", "binaen", "hitaben", "istinaden", "mahsuben", "mukabil", "nazaran"}, 19)) {
                                                                                                    result = create_string3(form, "+POSTP+PCDAT");
                                                                                                } else {
                                                                                                    if (string_in_list(form, (char*[]) {"sonra", "önce", "beri", "fazla", "dolayı", "itibaren", "başka", "çok", "evvel", "ötürü", "yana", "öte", "aşağı", "yukarı", "dışarı", "az", "gayrı"}, 17)) {
                                                                                                        result = create_string3(form, "+POSTP+PCABL");
                                                                                                    } else {
                                                                                                        if (strcmp(form, "yanısıra") == 0) {
                                                                                                            result = create_string3(form, "+POSTP+PCGEN");
                                                                                                        } else {
                                                                                                            if (string_in_list(form, (char*[]) {"birlikte", "beraber"}, 2)) {
                                                                                                                result = create_string3(form, "+POSTP+PCINS");
                                                                                                            } else {
                                                                                                                if (string_in_list(form, (char*[]) {"aşkın", "takiben"}, 2)) {
                                                                                                                    result = create_string3(form, "+POSTP+PCACC");
                                                                                                                } else {
                                                                                                                    result = create_string3(form, "+POSTP+PCNOM");
                                                                                                                }
                                                                                                            }
                                                                                                        }
                                                                                                    }
                                                                                                }
                                                                                            } else {
                                                                                                if (starts_with(suffix, "PronounRoot")) {
                                                                                                    result = pronoun_transition(fsm_parse);
                                                                                                } else {
                                                                                                    if (strcmp(suffix, "OrdinalRoot") == 0) {
                                                                                                        result = create_string3(form, "+NUM+ORD");
                                                                                                    } else {
                                                                                                        if (starts_with(suffix, "Adjective")) {
                                                                                                            result = create_string3(form, "+ADJ");
                                                                                                        }
                                                                                                    }
                                                                                                }
                                                                                            }
                                                                                        }
                                                                                    }
                                                                                }
                                                                            }
                                                                        }
                                                                    }
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    for (int i = 0; i < fsm_parse->transition_list->size; i++) {
        char* transition = array_list_get(fsm_parse->transition_list, i);
        if (transition != NULL) {
            if (!starts_with(transition, "^")) {
                string_append(result, "+");
                string_append(result, transition);
            } else {
                string_append(result, transition);
            }
        }
    }
    char* tmp = malloc((strlen(result->s) + 1) * sizeof(char));
    strcpy(tmp, result->s);
    free_string_ptr(result);
    return tmp;
}

/**
 * The suffixList method gets the first items of suffixList and formList and concatenates them with parenthesis and
 * assigns a String result. Then, loops through the formList and it the current ith item is not equal to previous
 * item it accumulates ith items of formList and suffixList to the result String.
 *
 * @return result String accumulated with the items of formList and suffixList.
 */
char *get_suffix_list(Fsm_parse_ptr fsm_parse) {
    char tmp[MAX_LINE_LENGTH];
    sprintf(tmp, "%s(%s)", ((Fsm_State_ptr) array_list_get(fsm_parse->suffix_list, 0))->name, (char*) array_list_get(fsm_parse->form_list, 0));
    for (int i = 1; i < fsm_parse->form_list->size; i++) {
        if (strcmp(array_list_get(fsm_parse->form_list, i), array_list_get(fsm_parse->form_list, i - 1)) != 0) {
            sprintf(tmp, "%s+%s(%s)", tmp, ((Fsm_State_ptr) array_list_get(fsm_parse->suffix_list, i))->name, (char*) array_list_get(fsm_parse->form_list, i));
        }
    }
    char *result = NULL;
    result = str_copy(result, tmp);
    return result;
}

/**
 * The withList method gets the root as a result String then loops through the withList and concatenates each item
 * with result String.
 *
 * @return result String accumulated with items of withList.
 */
char *get_with_list(Fsm_parse_ptr fsm_parse) {
    char tmp[MAX_LINE_LENGTH];
    sprintf(tmp, "%s", fsm_parse->root->word->name);
    for (int i = 0; i < fsm_parse->with_list->size; i++) {
        sprintf(tmp, "%s+%s", tmp, (char*) array_list_get(fsm_parse->with_list, i));
    }
    char *result = NULL;
    result = str_copy(result, tmp);
    return result;
}

/**
 * Replace root word of the current parse with the new root word and returns the new word.
 * @param newRoot Replaced root word
 * @return Root word of the parse will be replaced with the newRoot and the resulting surface form is returned.
 */
char *replace_root_word(Fsm_parse_ptr fsm_parse, Txt_word_ptr new_root) {
    char* result = NULL;
    result = str_copy(result, new_root->word->name);
    for (int i = 0; i < fsm_parse->with_list->size; i++) {
        Transition_ptr transition = create_transition3(array_list_get(fsm_parse->with_list, i));
        char* tmp = make_transition(transition, new_root, result);
        free(result);
        result = tmp;
        free_transition(transition);
    }
    return result;
}

/**
 * The overridden toString method which returns transitionList method.
 *
 * @return returns transitionList method.
 */
char *fsm_parse_to_string(Fsm_parse_ptr fsm_parse) {
    return transition_list(fsm_parse);
}

int compare_fsm_parse(const Fsm_parse *fsm_parse1, const Fsm_parse *fsm_parse2) {
    char* transition1 = transition_list(fsm_parse1);
    char* transition2 = transition_list(fsm_parse2);
    int result = compare_string(transition1, transition2);
    free(transition1);
    free(transition2);
    return result;
}

Word_ptr get_word_with_pos2(const Fsm_parse *fsm_parse) {
    char tmp[MAX_LINE_LENGTH];
    char *tag = get_tag(get_tag_with_index(first_inflectional_group2(fsm_parse), 0));
    sprintf(tmp, "%s+%s", fsm_parse->root->word->name, tag);
    free(tag);
    return create_word(tmp);
}

Inflectional_group_ptr first_inflectional_group2(const Fsm_parse *fsm_parse) {
    return array_list_get(fsm_parse->inflectional_groups, 0);
}

/**
 * The overridden clone method creates a new FsmParse abject with root variable and initializes variables form, pos,
 * initialPos, verbAgreement, possesiveAgreement, and also the vectors suffixList, formList, transitionList and withList.
 * Then returns newly created and cloned FsmParse object.
 *
 * @return FsmParse object.
 */
Fsm_parse_ptr clone_fsm_parse(const Fsm_parse *fsm_parse) {
    Fsm_parse_ptr p = create_fsm_parse(fsm_parse->root);
    p->form = str_copy(p->form, fsm_parse->form);
    p->pos = fsm_parse->pos;
    p->initial_pos = fsm_parse->initial_pos;
    p->verb_agreement = fsm_parse->verb_agreement;
    p->possessive_agreement = fsm_parse->possessive_agreement;
    p->suffix_list = clone_array_list(fsm_parse->suffix_list);
    p->form_list = clone_array_list(fsm_parse->form_list);
    p->transition_list = clone_array_list(fsm_parse->transition_list);
    p->with_list = clone_array_list(fsm_parse->with_list);
    p->inflectional_groups = clone_array_list(fsm_parse->inflectional_groups);
    return p;
}
