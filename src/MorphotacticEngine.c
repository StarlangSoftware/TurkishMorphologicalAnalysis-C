//
// Created by Olcay Taner YILDIZ on 18.10.2023.
//

#include <string.h>
#include <Language/TurkishLanguage.h>
#include "MorphotacticEngine.h"

/**
 * resolveD resolves the D metamorpheme to 'd' or 't' depending on the root and current formationToCheck. It adds
 * 'd' if the root is an abbreviation; 't' if the last phoneme is one of the "çfhkpsşt" (fıstıkçı şahap) or 'd'
 * otherwise; 't' if the word is a number ending with 3, 4, 5, 40, 60, or 70 or 'd' otherwise.
 * @param root Root of the word
 * @param formation Formation is current status of the wordform in the current state of the finite state machine. It
 *                  is always equal to formationToCheck except the case where there is an apostrophe after the
 *                  formationToCheck such as (3').
 * @param formation_to_check FormationToCheck is current status of the wordform in the current state of the finite
 *                         state machine except the apostrophe at the end if it exists.
 * @return Formation with added 'd' or 't' character.
 */
void resolve_D(Txt_word_ptr root,
               String_ptr formation,
               const char *formation_to_check) {
    char* added;
    if (is_abbreviation(root)) {
        string_append(formation, "d");
        return;
    }
    String_ptr lastPhoneme = last_phoneme(formation_to_check);
    if (strcmp(lastPhoneme->s, "0") >= 0 && strcmp(lastPhoneme->s, "9") <= 0) {
        if (string_equals2(lastPhoneme, "3") || string_equals2(lastPhoneme, "4") || string_equals2(lastPhoneme, "5")){
            //3->3'tü, 5->5'ti, 4->4'tü
            added = "t";
        } else {
            if (string_equals2(lastPhoneme, "0")){
                if (ends_with(root->name, "40") ||
                    ends_with(root->name, "60") ||
                    ends_with(root->name, "70"))
                    //40->40'tı, 60->60'tı, 70->70'ti
                    added = "t";
                else
                    //30->30'du, 50->50'ydi, 80->80'di
                    added = "d";
            } else {
                added = "d";
            }
        }
    } else {
        if (is_sert_sessiz(lastPhoneme->s)) {
            //yap+DH->yaptı
            added = "t";
        } else {
            //sar+DH->sardı
            added = "d";
        }
    }
    free_string_ptr(lastPhoneme);
    string_append(formation, added);
}

/**
 * resolveA resolves the A metamorpheme to 'a' or 'e' depending on the root and current formation_to_check. It adds
 * 'e' if the root is an abbreviation; 'a' if the last vowel is a back vowel (except words that do not obey vowel
 * harmony during agglutination); 'e' if the last vowel is a front vowel (except words that do not obey vowel
 * harmony during agglutination); 'a' if the word is a number ending with 6, 9, 10, 30, 40, 60, or 90 or 'e'
 * otherwise.
 * @param root Root of the word
 * @param formation Formation is current status of the wordform in the current state of the finite state machine. It
 *                  is always equal to formation_to_check except the case where there is an apostrophe after the
 *                  formation_to_check such as (3').
 * @param root_word True if the current word form is root form, false otherwise.
 * @param formation_to_check FormationToCheck is current status of the wordform in the current state of the finite
 *                         state machine except the apostrophe at the end if it exists.
 * @return Formation with added 'a' or 'e' character.
 */
void resolve_A(Txt_word_ptr root,
               String_ptr formation,
               bool root_word,
               const char *formation_to_check) {
    char* added = "";
    if (is_abbreviation(root)) {
        string_append(formation, "e");
        return;
    }
    String_ptr lastVowel = last_vowel(formation_to_check);
    if (strcmp(lastVowel->s, "0") >= 0 && strcmp(lastVowel->s, "9") <= 0) {
        if (string_equals2(lastVowel, "6") || string_equals2(lastVowel, "9")){
            //6'ya, 9'a
            added = "a";
        } else {
            if (string_equals2(lastVowel, "0")){
                if (ends_with(root->name, "10") ||
                    ends_with(root->name, "30") ||
                    ends_with(root->name, "40") ||
                    ends_with(root->name, "60") ||
                    ends_with(root->name, "90"))
                    //10'a, 30'a, 40'a, 60'a, 90'a
                    added = "a";
                else
                    //20'ye, 50'ye, 80'e, 70'e
                    added = "e";
            } else {
                //3'e, 8'e, 4'e, 2'ye
                added = "e";
            }
        }
    }
    if (is_back_vowel(lastVowel->s)) {
        if (not_obeys_vowel_harmony_during_agglutination(root) && root_word) {
            //alkole, anormale, ampule, tümamirali, spirali, sosyali
            added = "e";
        } else {
            //sakala, kabala, eve, kediye
            added = "a";
        }
    }
    if (is_front_vowel(lastVowel->s)) {
        if (not_obeys_vowel_harmony_during_agglutination(root) && root_word) {
            //sakala, kabala, eve, kediye
            added = "a";
        } else {
            //alkole, anormale, ampule, tümamirali, spirali, sosyali
            added = "e";
        }
    }
    if (is_numeral(root) || is_fraction(root) || is_real(root)) {
        if (ends_with(root->name, "6") ||
            ends_with(root->name, "9") ||
            ends_with(root->name, "10") ||
            ends_with(root->name, "30") ||
            ends_with(root->name, "40") ||
            ends_with(root->name, "60") ||
            ends_with(root->name, "90")) {
            added = "a";
        } else {
            added = "e";
        }
    }
    free_string_ptr(lastVowel);
    string_append(formation, added);
}

/**
 * resolveH resolves the H metamorpheme to 'ı', 'i', 'u' or 'ü', depending on the  current formationToCheck, root,
 * and formation. It adds 'i' if the root is an abbreviation; 'ü' if the  character before the last vowel is
 * front rounded (or back rounded when the root word does not obey vowel harmony during agglutination); 'i' if the
 * character before the last vowel is front unrounded; 'u' if the character before the  last vowel is back rounded;
 * 'ı' if the character before the last vowel is back unrounded (or front unrounded when the root word does not obey
 * vowel harmony during agglutination); 'ı' if the word is a  number ending with 6, 40, 60 or 90; 'ü' if the word
 * is a number ending with 3, 4, or 00; 'u' if the word is a number ending with 9, 10, or 30; 'i' otherwise for
 * numbers. Special case for 'Hyor' suffix is handled with resolveHforSpecialCaseTenseSuffix method.
 * @param root Root of the word
 * @param formation Formation is current status of the wordform in the current state of the finite state machine. It
 *                  is always equal to formationToCheck except the case where there is an apostrophe after the
 *                  formationToCheck such as (3').
 * @param beginning_of_suffix True if H appears in the beginning of the suffix, false otherwise.
 * @param special_case_tense_suffix True if the suffix is 'Hyor', false otherwise.
 * @param root_word True if the current word form is root form, false otherwise.
 * @param formation_to_check FormationToCheck is current status of the word form in the current state of the finite
 *                         state machine except the apostrophe at the end if it exists.
 * @return Formation with possibly last character dropped and 'ı', 'i', 'u' or 'ü' character added.
 */
void resolve_H(Txt_word_ptr root,
               String_ptr formation,
               bool beginning_of_suffix,
               bool special_case_tense_suffix,
               bool root_word,
               const char *formation_to_check) {
    char except_last_char[MAX_WORD_LENGTH];
    bool vowel, front_rounded, front_unrounded, back_rounded, back_unrounded;
    if (is_abbreviation(root)) {
        string_append(formation, "i");
        return;
    }
    String_ptr last_p = last_phoneme(formation_to_check);
    vowel = is_vowel(last_p->s);
    free_string_ptr(last_p);
    if (beginning_of_suffix && vowel && !special_case_tense_suffix) {
        return;
    }
    if (special_case_tense_suffix) {
        //eğer ek Hyor eki ise,
        String_ptr before_last_v = before_last_vowel(formation_to_check);
        front_rounded = is_front_rounded_vowel(before_last_v->s);
        front_unrounded = is_front_unrounded_vowel(before_last_v->s);
        back_rounded = is_back_rounded_vowel(before_last_v->s);
        back_unrounded = is_back_unrounded_vowel(before_last_v->s);
        free_string_ptr(before_last_v);
        String_ptr tmp = substring_except_last_char(formation->s);
        strcpy(except_last_char, tmp->s);
        free_string_ptr(tmp);
        if (root_word) {
            if (vowel_a_changes_to_i_during_y_suffixation(root)) {
                if (front_rounded) {
                    //büyülüyor, bölümlüyor, çözümlüyor, döşüyor
                    string_replace(formation, strcat(except_last_char, "ü"));
                    return;
                }
                if (front_unrounded) {
                    //adresliyor, alevliyor, ateşliyor, bekliyor
                    string_replace(formation, strcat(except_last_char, "i"));
                    return;
                }
                if (back_rounded) {
                    //buğuluyor, bulguluyor, çamurluyor, aforozluyor
                    string_replace(formation, strcat(except_last_char, "u"));
                    return;
                }
                if (back_unrounded) {
                    //açıklıyor, çalkalıyor, gazlıyor, gıcırdıyor
                    string_replace(formation, strcat(except_last_char, "ı"));
                    return;
                }
            }
        }
        if (vowel) {
            if (front_rounded) {
                string_replace(formation, strcat(except_last_char, "ü"));
                return;
            }
            if (front_unrounded) {
                string_replace(formation, strcat(except_last_char, "i"));
                return;
            }
            if (back_rounded) {
                string_replace(formation, strcat(except_last_char, "u"));
                return;
            }
            if (back_unrounded) {
                string_replace(formation, strcat(except_last_char, "ı"));
                return;
            }
        }
    }
    String_ptr last_v = last_vowel(formation_to_check);
    bool last_vowel_empty = (strcmp(last_v->s, "0") == 0);
    front_rounded = is_front_rounded_vowel(last_v->s);
    front_unrounded = is_front_unrounded_vowel(last_v->s);
    back_rounded = is_back_rounded_vowel(last_v->s);
    back_unrounded = is_back_unrounded_vowel(last_v->s);
    if ((front_unrounded && (!not_obeys_vowel_harmony_during_agglutination(root) || !root_word)) || (string_equals2(last_v, "a") && not_obeys_vowel_harmony_during_agglutination(root))) {
        free_string_ptr(last_v);
        string_append(formation, "i");
        return;
    }
    free_string_ptr(last_v);
    if (front_rounded || (back_rounded && not_obeys_vowel_harmony_during_agglutination(root))) {
        string_append(formation, "ü");
        return;
    }
    if (back_rounded) {
        string_append(formation, "u");
        return;
    }
    if (back_unrounded || (front_unrounded && not_obeys_vowel_harmony_during_agglutination(root))) {
        string_append(formation, "ı");
        return;
    }
    if (is_numeral(root) || is_fraction(root) || is_real(root)) {
        if (ends_with(root->name, "6") ||
            ends_with(root->name, "40") ||
            ends_with(root->name, "60") ||
            ends_with(root->name, "90")) {
            //6'yı, 40'ı, 60'ı
            string_append(formation, "ı");
            return;
        } else {
            if (ends_with(root->name, "3") ||
                ends_with(root->name, "4") ||
                ends_with(root->name, "00")) {
                //3'ü, 4'ü, 100'ü
                string_append(formation, "ü");
                return;
            } else {
                if (ends_with(root->name, "9") ||
                    ends_with(root->name, "10") ||
                    ends_with(root->name, "30")) {
                    //9'u, 10'u, 30'u
                    string_append(formation, "u");
                    return;
                } else {
                    //2'yi, 5'i, 8'i
                    string_append(formation, "i");
                    return;
                }
            }
        }
    }
    if (last_vowel_empty){
        string_append(formation, "i");
    }
}

/**
 * The resolveC method takes a String formation as an input. If the last phoneme is on of the "çfhkpsşt", it
 * concatenates given formation with 'ç', if not it concatenates given formation with 'c'.
 *
 * @param formation String input.
 * @return resolved String.
 */
void resolve_C(String_ptr formation, const char *formationToCheck) {
    char* added;
    String_ptr lastPhoneme = last_phoneme(formationToCheck);
    if (is_sert_sessiz(lastPhoneme->s)) {
        added = "ç";
    } else {
        added = "c";
    }
    free_string_ptr(lastPhoneme);
    string_append(formation, added);
}

/**
 * The resolveS method takes a String formation as an input. It then concatenates given formation with 's'.
 *
 * @param formation String input.
 * @return resolved String.
 */
void resolve_S(String_ptr formation) {
    string_append(formation, "s");
}

/**
 * The resolveSh method takes a String formation as an input. If the last character is a vowel, it concatenates
 * given formation with ş, if the last character is not a vowel, and not 't' it directly returns given formation, but if it
 * is equal to 't', it transforms it to 'd'.
 *
 * @param formation String input.
 * @return resolved String.
 */
void resolve_Sh(String_ptr formation) {
    String_ptr lastChar = last_char(formation->s);
    if (is_vowel(lastChar->s)) {
        string_append(formation, "ş");
    } else {
        if (string_equals2(lastChar, "t")) {
            String_ptr tmp = substring_except_last_char(formation->s);
            string_replace(formation, tmp->s);
            free_string_ptr(tmp);
            string_append(formation, "d");
        }
    }
    free_string_ptr(lastChar);
}
