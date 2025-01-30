//
// Created by Olcay Taner YILDIZ on 16.10.2023.
//

#include <string.h>
#include <FileUtils.h>
#include <CounterHashMap.h>
#include <Memory/Memory.h>
#include "MorphologicalParse.h"
#include "InflectionalGroup.h"
#include "Language/TurkishLanguage.h"

/**
 * Another constructor of MorphologicalParse class which takes a String parse as an input. First it creates
 * an ArrayList as iGs for inflectional groups, and while given String contains derivational boundary (^DB+), it
 * adds the substring to the iGs ArrayList and continue to use given String from 4th index. If it does not contain ^DB+,
 * it directly adds the given String to the iGs ArrayList. Then, it creates a new ArrayList as
 * inflectionalGroups and checks for some cases.
 * <p>
 * If the first item of iGs ArrayList is ++Punc, it creates a new root as +, and by calling
 * InflectionalGroup method with Punc it initializes the IG ArrayList by parsing given input
 * String IG by + and calling the getMorphologicalTag method with these substrings. If getMorphologicalTag method returns
 * a tag, it adds this tag to the IG ArrayList and also to the inflectionalGroups ArrayList.
 * <p>
 * If the first item of iGs ArrayList has +, it creates a new word of first item's substring from index 0 to +,
 * and assigns it to root. Then, by calling InflectionalGroup method with substring from index 0 to +,
 * it initializes the IG ArrayList by parsing given input String IG by + and calling the getMorphologicalTag
 * method with these substrings. If getMorphologicalTag method returns a tag, it adds this tag to the IG ArrayList
 * and also to the inflectionalGroups ArrayList.
 * <p>
 * If the first item of iGs ArrayList does not contain +, it creates a new word with first item and assigns it as root.
 * <p>
 * At the end, it loops through the items of iGs and by calling InflectionalGroup method with these items
 * it initializes the IG ArrayList by parsing given input String IG by + and calling the getMorphologicalTag
 * method with these substrings. If getMorphologicalTag method returns a tag, it adds this tag to the IG ArrayList
 * and also to the inflectionalGroups ArrayList.
 *
 * @param parse String input.
 */
Morphological_parse_ptr create_morphological_parse(const char *parse) {
    Morphological_parse_ptr result = malloc_(sizeof(Morphological_parse), "create_morphological_parse");
    result->inflectional_groups = create_array_list();
    Array_list_ptr iGs = str_split2(parse, "^DB+");
    if (strcmp(((char *) array_list_get(iGs, 0)), "++Punc") == 0) {
        result->root = str_copy(result->root, "+");
        array_list_add(result->inflectional_groups, create_inflectional_group("Punc"));
    } else {
        update_root_and_inflectional_groups(result, iGs);
    }
    free_array_list(iGs, free_);
    return result;
}

/**
 * Another constructor of MorphologicalParse class which takes an vector inflectionalGroups as an input.
 *
 * @param inflectional_groups vector input.
 */
Morphological_parse_ptr create_morphological_parse2(const Array_list *inflectional_groups) {
    Morphological_parse_ptr result = malloc_(sizeof(Morphological_parse), "create_morphological_parse2");
    result->inflectional_groups = create_array_list();
    update_root_and_inflectional_groups(result, inflectional_groups);
    return result;
}

/**
 * It initializes _inflectionalGroups ArrayList and if the first item of the vector has +, it gets
 * the substring from index 0 to + and assigns it as root, and by calling InflectionalGroup method with substring from index 0 to +,
 * it initializes the IG vector by parsing given input String IG by + and calling the getMorphologicalTag
 * method with these substrings. If getMorphologicalTag method returns a tag, it adds this tag to the IG vector
 * and also to the _inflectionalGroups vector. However, if the first item does not contain +, it directly prints out
 * indicating that there is no root for that item of this Inflectional Group.
 * <p>
 * At the end, it loops through the items of _inflectionalGroups and by calling InflectionalGroup method with these items
 * it initializes the IG vector by parsing given input String IG by + and calling the getMorphologicalTag
 * method with these substrings. If getMorphologicalTag method returns a tag, it adds this tag to the IG vector
 * and also to the _inflectionalGroups vector.
 *
 * @param morphological_parse Current morphological parse object
 * @param inflectional_groups vector input.
 */
void update_root_and_inflectional_groups(Morphological_parse_ptr morphological_parse,
                                         const Array_list *inflectional_groups) {
    if (str_contains(array_list_get(inflectional_groups, 0), "+")) {
        String_ptr s1 = substring(array_list_get(inflectional_groups, 0), 0,
                                  str_find_c(array_list_get(inflectional_groups, 0), "+"));
        morphological_parse->root = str_copy(morphological_parse->root, s1->s);
        free_string_ptr(s1);
        String_ptr s2 = substring2(array_list_get(inflectional_groups, 0),
                                   str_find_c(array_list_get(inflectional_groups, 0), "+"));
        array_list_add(morphological_parse->inflectional_groups, create_inflectional_group(s2->s));
        free_string_ptr(s2);
    } else {
        morphological_parse->root = str_copy(morphological_parse->root, array_list_get(inflectional_groups, 0));
    }
    for (int i = 1; i < inflectional_groups->size; i++) {
        array_list_add(morphological_parse->inflectional_groups,
                       create_inflectional_group(array_list_get(inflectional_groups, i)));
    }
}

/**
 * The getTransitionList method gets the first item of inflectionalGroups ArrayList as a String, then loops
 * through the items of inflectionalGroups and concatenates them by using +.
 *
 * @param morphological_parse Current morphological parse object
 * @return String that contains transition list.
 */
char *get_transition_list(const Morphological_parse *morphological_parse) {
    char tmp[MAX_LINE_LENGTH];
    char *ig;
    ig = inflectional_group_to_string(array_list_get(morphological_parse->inflectional_groups, 0));
    sprintf(tmp, "%s", ig);
    free_(ig);
    for (int i = 1; i < morphological_parse->inflectional_groups->size; i++) {
        ig = inflectional_group_to_string(array_list_get(morphological_parse->inflectional_groups, i));
        sprintf(tmp, "%s+%s", tmp, ig);
        free_(ig);
    }
    char *result = NULL;
    result = str_copy(result, tmp);
    return result;
}

/**
 * The getInflectionalGroupString method takes an Integer index as an input and if index is 0, it directly returns the
 * root and the first item of inflectionalGroups vector. If the index is not 0, it then returns the corresponding
 * item of inflectionalGroups vector as a String.
 *
 * @param morphological_parse Current morphological parse object
 * @param index Integer input.
 * @return corresponding item of inflectionalGroups at given index as a String.
 */
char *get_inflectional_group_string(const Morphological_parse *morphological_parse, int index) {
    char tmp[MAX_LINE_LENGTH];
    if (index == 0) {
        char *ig = inflectional_group_to_string(array_list_get(morphological_parse->inflectional_groups, 0));
        sprintf(tmp, "%s+%s", morphological_parse->root, ig);
        free_(ig);
        char *result = NULL;
        result = str_copy(result, tmp);
        return result;
    } else {
        return inflectional_group_to_string(array_list_get(morphological_parse->inflectional_groups, index));
    }
}

/**
 * The getInflectionalGroup method takes an Integer index as an input and it directly returns the InflectionalGroup
 * at given index.
 *
 * @param morphological_parse Current morphological parse object
 * @param index Integer input.
 * @return InflectionalGroup at given index.
 */
Inflectional_group_ptr get_inflectional_group(const Morphological_parse *morphological_parse, int index) {
    return array_list_get(morphological_parse->inflectional_groups, index);
}

/**
 * The getLastInflectionalGroup method directly returns the last InflectionalGroup of inflectionalGroups ArrayList.
 *
 * @param morphological_parse Current morphological parse object
 * @return the last InflectionalGroup of inflectionalGroups ArrayList.
 */
Inflectional_group_ptr get_last_inflectional_group(const Morphological_parse *morphological_parse) {
    return array_list_get(morphological_parse->inflectional_groups, morphological_parse->inflectional_groups->size - 1);
}

/**
 * The getTag method takes an Integer index as an input and and if the given index is 0, it directly return the root.
 * then, it loops through the inflectionalGroups ArrayList it returns the MorphologicalTag of the corresponding inflectional group.
 *
 * @param morphological_parse Current morphological parse object
 * @param index Integer input.
 * @return the MorphologicalTag of the corresponding inflectional group, or null of invalid index inputs.
 */
char *get_tag_for_index(const Morphological_parse *morphological_parse, int index) {
    int size = 1;
    if (index == 0)
        return morphological_parse->root;
    for (int i = 0; i < morphological_parse->inflectional_groups->size; i++) {
        Inflectional_group_ptr group = array_list_get(morphological_parse->inflectional_groups, i);
        if (index < size + group->IG->size) {
            return get_tag(get_tag_with_index(group, index - size));
        }
        size += group->IG->size;
    }
    return NULL;
}

/**
 * The tagSize method loops through the inflectionalGroups ArrayList and accumulates the sizes of each inflectional group
 * in the inflectionalGroups.
 *
 * @param morphological_parse Current morphological parse object
 * @return total size of the inflectionalGroups ArrayList.
 */
int tag_size(const Morphological_parse *morphological_parse) {
    int size = 1;
    for (int i = 0; i < morphological_parse->inflectional_groups->size; i++) {
        Inflectional_group_ptr group = array_list_get(morphological_parse->inflectional_groups, i);
        size += group->IG->size;
    }
    return size;
}

/**
 * The firstInflectionalGroup method returns the first inflectional group of inflectionalGroups ArrayList.
 *
 * @param morphological_parse Current morphological parse object
 * @return the first inflectional group of inflectionalGroups ArrayList.
 */
Inflectional_group_ptr first_inflectional_group(const Morphological_parse *morphological_parse) {
    return array_list_get(morphological_parse->inflectional_groups, 0);
}

/**
 * The lastInflectionalGroup method returns the last inflectional group of inflectionalGroups ArrayList.
 *
 * @param morphological_parse Current morphological parse object
 * @return the last inflectional group of inflectionalGroups ArrayList.
 */
Inflectional_group_ptr last_inflectional_group(const Morphological_parse *morphological_parse) {
    return array_list_get(morphological_parse->inflectional_groups, morphological_parse->inflectional_groups->size - 1);
}

/**
 * The getWordWithPos method returns root with the MorphologicalTag of the first inflectional as a new word.
 *
 * @param morphological_parse Current morphological parse object
 * @return root with the MorphologicalTag of the first inflectional as a new word.
 */
char *get_word_with_pos(const Morphological_parse *morphological_parse) {
    char tmp[MAX_LINE_LENGTH];
    char *tag = get_tag(get_tag_with_index(first_inflectional_group(morphological_parse), 0));
    sprintf(tmp, "%s+%s", morphological_parse->root, tag);
    free_(tag);
    return clone_string(tmp);
}

/**
 * The getPos method returns the MorphologicalTag of the last inflectional group.
 *
 * @param morphological_parse Current morphological parse object
 * @return the MorphologicalTag of the last inflectional group.
 */
char *get_pos(const Morphological_parse *morphological_parse) {
    return get_tag(get_tag_with_index(last_inflectional_group(morphological_parse), 0));
}

/**
 * The getRootPos method returns the MorphologicalTag of the first inflectional group.
 *
 * @param morphological_parse Current morphological parse object
 * @return the MorphologicalTag of the first inflectional group.
 */
char *get_root_pos(const Morphological_parse *morphological_parse) {
    return get_tag(get_tag_with_index(first_inflectional_group(morphological_parse), 0));
}

/**
 * The lastIGContainsCase method returns the MorphologicalTag of last inflectional group if it is one of the NOMINATIVE,
 * ACCUSATIVE, DATIVE, LOCATIVE or ABLATIVE cases, null otherwise.
 *
 * @param morphological_parse Current morphological parse object
 * @return the MorphologicalTag of last inflectional group if it is one of the NOMINATIVE,
 * ACCUSATIVE, DATIVE, LOCATIVE or ABLATIVE cases, null otherwise.
 */
char *last_ig_contains_case(const Morphological_parse *morphological_parse) {
    Morphological_tag case_tag = contains_case(last_inflectional_group(morphological_parse));
    if (case_tag != NOTAG)
        return get_tag(case_tag);
    else
        return "NULL";
}

/**
 * The lastIGContainsTag method takes a MorphologicalTag as an input and returns true if the last inflectional group's
 * MorphologicalTag matches with one of the tags in the IG ArrayList, falze otherwise.
 *
 * @param morphological_parse Current morphological parse object
 * @param tag MorphologicalTag type input.
 * @return true if the last inflectional group's MorphologicalTag matches with one of the tags in the IG ArrayList, false otherwise.
 */
bool last_ig_contains_tag(const Morphological_parse *morphological_parse, Morphological_tag tag) {
    return contains_tag(last_inflectional_group(morphological_parse), tag);
}

/**
 * lastIGContainsPossessive method returns true if the last inflectional group contains one of the
 * possessives: P1PL, P1SG, P2PL, P2SG, P3PL AND P3SG, false otherwise.
 *
 * @param morphological_parse Current morphological parse object
 * @return true if the last inflectional group contains one of the possessives: P1PL, P1SG, P2PL, P2SG, P3PL AND P3SG, false otherwise.
 */
bool last_ig_contains_possessive(const Morphological_parse *morphological_parse) {
    return contains_possessive(last_inflectional_group(morphological_parse));
}

/**
 * The isCapitalWord method returns true if the character at first index o f root is an uppercase letter, false otherwise.
 *
 * @param morphological_parse Current morphological parse object
 * @return true if the character at first index o f root is an uppercase letter, false otherwise.
 */
bool is_capital_word(const Morphological_parse *morphological_parse) {
    String_ptr st = char_at(morphological_parse->root, 0);
    bool result = is_uppercase(st->s);
    free_string_ptr(st);
    return result;
}

/**
 * The isNoun method returns true if the past of speech is NOUN, false otherwise.
 *
 * @param morphological_parse Current morphological parse object
 * @return true if the past of speech is NOUN, false otherwise.
 */
bool is_noun(const Morphological_parse *morphological_parse) {
    char *pos = get_pos(morphological_parse);
    bool result = strcmp(pos, "NOUN") == 0;
    free_(pos);
    return result;
}

/**
 * The isVerb method returns true if the past of speech is VERB, false otherwise.
 *
 * @param morphological_parse Current morphological parse object
 * @return true if the past of speech is VERB, false otherwise.
 */
bool is_parse_verb(const Morphological_parse *morphological_parse) {
    char *pos = get_pos(morphological_parse);
    bool result = strcmp(pos, "VERB") == 0;
    free_(pos);
    return result;
}

/**
 * The isRootVerb method returns true if the past of speech of root is BERV, false otherwise.
 *
 * @param morphological_parse Current morphological parse object
 * @return true if the past of speech of root is VERB, false otherwise.
 */
bool is_root_verb(const Morphological_parse *morphological_parse) {
    char *pos = get_root_pos(morphological_parse);
    bool result = strcmp(pos, "VERB") == 0;
    free_(pos);
    return result;
}

/**
 * The isAdjective method returns true if the past of speech is ADJ, false otherwise.
 *
 * @param morphological_parse Current morphological parse object
 * @return true if the past of speech is ADJ, false otherwise.
 */
bool is_parse_adjective(const Morphological_parse *morphological_parse) {
    char *pos = get_pos(morphological_parse);
    bool result = strcmp(pos, "ADJ") == 0;
    free_(pos);
    return result;
}

/**
 * The isProperNoun method returns true if the first inflectional group's MorphologicalTag is a PROPERNOUN, false otherwise.
 *
 * @param morphological_parse Current morphological parse object
 * @return true if the first inflectional group's MorphologicalTag is a PROPERNOUN, false otherwise.
 */
bool is_parse_proper_noun(const Morphological_parse *morphological_parse) {
    return contains_tag(get_inflectional_group(morphological_parse, 0), PROPERNOUN);
}

/**
 * The isPunctuation method returns true if the first inflectional group's MorphologicalTag is a PUNCTUATION, false otherwise.
 *
 * @param morphological_parse Current morphological parse object
 * @return true if the first inflectional group's MorphologicalTag is a PUNCTUATION, false otherwise.
 */
bool is_parse_punctuation(const Morphological_parse *morphological_parse) {
    return contains_tag(get_inflectional_group(morphological_parse, 0), PUNCTUATION);
}

/**
 * The isCardinal method returns true if the first inflectional group's MorphologicalTag is a CARDINAL, false otherwise.
 *
 * @param morphological_parse Current morphological parse object
 * @return true if the first inflectional group's MorphologicalTag is a CARDINAL, false otherwise.
 */
bool is_cardinal(const Morphological_parse *morphological_parse) {
    return contains_tag(get_inflectional_group(morphological_parse, 0), CARDINAL);
}

/**
 * The isOrdinal method returns true if the first inflectional group's MorphologicalTag is a ORDINAL, false otherwise.
 *
 * @param morphological_parse Current morphological parse object
 * @return true if the first inflectional group's MorphologicalTag is a ORDINAL, false otherwise.
 */
bool is_parse_ordinal(const Morphological_parse *morphological_parse) {
    return contains_tag(get_inflectional_group(morphological_parse, 0), ORDINAL);
}

/**
 * The isReal method returns true if the first inflectional group's MorphologicalTag is a REAL, false otherwise.
 *
 * @param morphological_parse Current morphological parse object
 * @return true if the first inflectional group's MorphologicalTag is a REAL, false otherwise.
 */
bool is_parse_real(const Morphological_parse *morphological_parse) {
    return contains_tag(get_inflectional_group(morphological_parse, 0), REAL);
}

/**
 * The isNumber method returns true if the first inflectional group's MorphologicalTag is REAL or CARDINAL, false otherwise.
 *
 * @param morphological_parse Current morphological parse object
 * @return true if the first inflectional group's MorphologicalTag is a REAL or CARDINAL, false otherwise.
 */
bool is_number(const Morphological_parse *morphological_parse) {
    return is_parse_real(morphological_parse) || is_cardinal(morphological_parse);
}

/**
 * The isTime method returns true if the first inflectional group's MorphologicalTag is a TIME, false otherwise.
 *
 * @param morphological_parse Current morphological parse object
 * @return true if the first inflectional group's MorphologicalTag is a TIME, false otherwise.
 */
bool is_parse_time(const Morphological_parse *morphological_parse) {
    return contains_tag(get_inflectional_group(morphological_parse, 0), TIME);
}

/**
 * The isDate method returns true if the first inflectional group's MorphologicalTag is a DATE, false otherwise.
 *
 * @param morphological_parse Current morphological parse object
 * @return true if the first inflectional group's MorphologicalTag is a DATE, false otherwise.
 */
bool is_parse_date(const Morphological_parse *morphological_parse) {
    return contains_tag(get_inflectional_group(morphological_parse, 0), DATE);
}

/**
 * The isHashTag method returns true if the first inflectional group's MorphologicalTag is a HASHTAG, false otherwise.
 *
 * @param morphological_parse Current morphological parse object
 * @return true if the first inflectional group's MorphologicalTag is a HASHTAG, false otherwise.
 */
bool is_hash_tag(const Morphological_parse *morphological_parse) {
    return contains_tag(get_inflectional_group(morphological_parse, 0), HASHTAG);
}

/**
 * The isEmail method returns true if the first inflectional group's MorphologicalTag is a EMAIL, false otherwise.
 *
 * @param morphological_parse Current morphological parse object
 * @return true if the first inflectional group's MorphologicalTag is a EMAIL, false otherwise.
 */
bool is_email(const Morphological_parse *morphological_parse) {
    return contains_tag(get_inflectional_group(morphological_parse, 0), EMAIL);
}

/**
 * The isPercent method returns true if the first inflectional group's MorphologicalTag is a PERCENT, false otherwise.
 *
 * @param morphological_parse Current morphological parse object
 * @return true if the first inflectional group's MorphologicalTag is a PERCENT, false otherwise.
 */
bool is_parse_percent(const Morphological_parse *morphological_parse) {
    return contains_tag(get_inflectional_group(morphological_parse, 0), PERCENT);
}

/**
 * The isFraction method returns true if the first inflectional group's MorphologicalTag is a FRACTION, false otherwise.
 *
 * @param morphological_parse Current morphological parse object
 * @return true if the first inflectional group's MorphologicalTag is a FRACTION, false otherwise.
 */
bool is_parse_fraction(const Morphological_parse *morphological_parse) {
    return contains_tag(get_inflectional_group(morphological_parse, 0), FRACTION);
}

/**
 * The isRange method returns true if the first inflectional group's MorphologicalTag is a RANGE, false otherwise.
 *
 * @param morphological_parse Current morphological parse object
 * @return true if the first inflectional group's MorphologicalTag is a RANGE, false otherwise.
 */
bool is_parse_range(const Morphological_parse *morphological_parse) {
    return contains_tag(get_inflectional_group(morphological_parse, 0), RANGE);
}

/**
 * The isPlural method returns true if InflectionalGroup's MorphologicalTags are from the agreement plural
 * or possessive plural, i.e A1PL, A2PL, A3PL, P1PL, P2PL or P3PL, and false otherwise.
 *
 * @param morphological_parse Current morphological parse object
 * @return true if InflectionalGroup's MorphologicalTags are from the agreement plural or possessive plural.
 */
bool is_parse_plural(const Morphological_parse *morphological_parse) {
    for (int i = 0; i < morphological_parse->inflectional_groups->size; i++) {
        Inflectional_group_ptr inflectional_group = array_list_get(morphological_parse->inflectional_groups, i);
        if (contains_plural(inflectional_group)) {
            return true;
        }
    }
    return false;
}

/**
 * The isAuxiliary method returns true if the root equals to the et, ol, or yap, and false otherwise.
 *
 * @param morphological_parse Current morphological parse object
 * @return true if the root equals to the et, ol, or yap, and false otherwise.
 */
bool is_auxiliary(const Morphological_parse *morphological_parse) {
    return string_in_list(morphological_parse->root, (char *[]) {"et", "ol", "yap"}, 3);
}

/**
 * The containsTag method takes a MorphologicalTag as an input and loops through the inflectionalGroups ArrayList,
 * returns true if the input matches with on of the tags in the IG, false otherwise.
 *
 * @param morphological_parse Current morphological parse object
 * @param tag checked tag
 * @return true if the input matches with on of the tags in the IG, false otherwise.
 */
bool parse_contains_tag(const Morphological_parse *morphological_parse, Morphological_tag tag) {
    for (int i = 0; i < morphological_parse->inflectional_groups->size; i++) {
        Inflectional_group_ptr inflectional_group = array_list_get(morphological_parse->inflectional_groups, i);
        if (contains_tag(inflectional_group, tag)) {
            return true;
        }
    }
    return false;
}

/**
 * The getTreePos method returns the tree pos tag of a morphological analysis.
 *
 * @param morphological_parse Current morphological parse object
 * @return Tree pos tag of the morphological analysis in string form.
 */
char *get_tree_pos(const Morphological_parse *morphological_parse) {
    if (is_parse_proper_noun(morphological_parse)) {
        return "NP";
    } else {
        if (strcmp(morphological_parse->root, "değil") == 0) {
            return "NEG";
        } else {
            if (is_parse_verb(morphological_parse)) {
                if (last_ig_contains_tag(morphological_parse, ZERO)) {
                    return "NOMP";
                } else {
                    return "VP";
                }
            } else {
                if (is_parse_adjective(morphological_parse)) {
                    return "ADJP";
                } else {
                    if (is_noun(morphological_parse) || is_parse_percent(morphological_parse)) {
                        return "NP";
                    } else {
                        if (parse_contains_tag(morphological_parse, ADVERB)) {
                            return "ADVP";
                        } else {
                            if (is_number(morphological_parse) || is_parse_fraction(morphological_parse)) {
                                return "NUM";
                            } else {
                                if (parse_contains_tag(morphological_parse, POSTPOSITION)) {
                                    return "PP";
                                } else {
                                    if (parse_contains_tag(morphological_parse, CONJUNCTION)) {
                                        return "CONJP";
                                    } else {
                                        if (parse_contains_tag(morphological_parse, DETERMINER)) {
                                            return "DP";
                                        } else {
                                            if (parse_contains_tag(morphological_parse, INTERJECTION)) {
                                                return "INTJP";
                                            } else {
                                                if (parse_contains_tag(morphological_parse, QUESTIONPRONOUN)) {
                                                    return "WP";
                                                } else {
                                                    if (parse_contains_tag(morphological_parse, PRONOUN)) {
                                                        return "NP";
                                                    } else {
                                                        if (is_parse_punctuation(morphological_parse)) {
                                                            if (string_in_list(morphological_parse->root,
                                                                               (char *[]) {"!", "?"}, 2)) {
                                                                return ".";
                                                            } else {
                                                                if (string_in_list(morphological_parse->root,
                                                                                   (char *[]) {";", "-", "--"}, 3)) {
                                                                    return ":";
                                                                } else {
                                                                    if (string_in_list(morphological_parse->root,
                                                                                       (char *[]) {"(", "-LRB-",
                                                                                                   "-lrb-"}, 3)) {
                                                                        return "-LRB-";
                                                                    } else {
                                                                        if (string_in_list(morphological_parse->root,
                                                                                           (char *[]) {")", "-RRB-",
                                                                                                       "-rrb-"}, 3)) {
                                                                            return "-rrb-";
                                                                        } else {
                                                                            return morphological_parse->root;
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
    return "-XXX-";
}

/**
 * Returns the pronoun type of the parse for universal dependency feature ProType.
 * @param morphological_parse Current morphological parse object
 * @return "Art" if the pronoun is also a determiner; "Prs" if the pronoun is personal pronoun; "Rcp" if the
 * pronoun is 'birbiri'; "Ind" if the pronoun is an indeterminate pronoun; "Neg" if the pronoun is 'hiçbiri';
 * "Int" if the pronoun is a question pronoun; "Dem" if the pronoun is a demonstrative pronoun.
 */
char *get_pron_type(const Morphological_parse *morphological_parse) {
    char *lemma = morphological_parse->root;
    if (parse_contains_tag(morphological_parse, DETERMINER)) {
        return "Art";
    }
    if (strcmp(lemma, "kendi") == 0 || parse_contains_tag(morphological_parse, PERSONALPRONOUN)) {
        return "Prs";
    }
    if (strcmp(lemma, "birbiri") == 0 || strcmp(lemma, "birbirleri") == 0) {
        return "Rcp";
    }
    if (string_in_list(lemma, (char *[]) {"birçoğu", "hep", "kimse", "bazı",
                                          "biri", "çoğu", "hepsi", "diğeri",
                                          "tümü", "herkes", "kimi", "öbür",
                                          "öteki", "birkaçı", "topu", "başkası"}, 16)) {
        return "Int";
    }
    if (string_in_list(lemma, (char *[]) {"kim", "nere", "ne", "hangi",
                                          "nasıl", "kaç", "mi", "mı",
                                          "mu", "mü"}, 10)) {
        return "Int";
    }
    if (parse_contains_tag(morphological_parse, DEMONSTRATIVEPRONOUN)) {
        return "Dem";
    }
    return NULL;
}

/**
 * Returns the numeral type of the parse for universal dependency feature NumType.
 * @param morphological_parse Current morphological parse object
 * @return "Ord" if the parse is Time, Ordinal or the word is '%' or 'kaçıncı'; "Dist" if the word is a
 * distributive number such as 'beşinci'; "Card" if the number is cardinal or any number or the word is 'kaç'.
 */
char *get_num_type(const Morphological_parse *morphological_parse) {
    char *lemma = morphological_parse->root;
    if (parse_contains_tag(morphological_parse, TIME) || strcmp(lemma, "%") == 0) {
        return "Ord";
    }
    if (parse_contains_tag(morphological_parse, ORDINAL) || strcmp(lemma, "kaçıncı") == 0) {
        return "Ord";
    }
    if (parse_contains_tag(morphological_parse, DISTRIBUTIVE)) {
        return "Dist";
    }
    if (parse_contains_tag(morphological_parse, CARDINAL) || parse_contains_tag(morphological_parse, NUMBER) ||
        strcmp(lemma, "kaç") == 0) {
        return "Card";
    }
    return NULL;
}

/**
 * Returns the value for the dependency feature Reflex.
 * @param morphological_parse Current morphological parse object
 * @return "Yes" if the root word is 'kendi', null otherwise.
 */
char *get_reflex(const Morphological_parse *morphological_parse) {
    char *lemma = morphological_parse->root;
    if (strcmp(lemma, "kendi") == 0) {
        return "Yes";
    }
    return NULL;
}

/**
 * Returns the agreement of the parse for the universal dependency feature Number.
 * @param morphological_parse Current morphological parse object
 * @return "Sing" if the agreement of the parse is singular (contains A1SG, A2SG, A3SG); "Plur" if the agreement
 * of the parse is plural (contains A1PL, A2PL, A3PL).
 */
char *get_number(const Morphological_parse *morphological_parse) {
    if (parse_contains_tag(morphological_parse, A1SG) || parse_contains_tag(morphological_parse, A2SG) ||
        parse_contains_tag(morphological_parse, A3SG)
        || parse_contains_tag(morphological_parse, P1SG) || parse_contains_tag(morphological_parse, P2SG) ||
        parse_contains_tag(morphological_parse, P3SG)) {
        return "Sing";
    }
    if (parse_contains_tag(morphological_parse, A1PL) || parse_contains_tag(morphological_parse, A2PL) ||
        parse_contains_tag(morphological_parse, A3PL)
        || parse_contains_tag(morphological_parse, P1PL) || parse_contains_tag(morphological_parse, P2PL) ||
        parse_contains_tag(morphological_parse, P3PL)) {
        return "Plur";
    }
    return NULL;
}

/**
 * Returns the possessive agreement of the parse for the universal dependency feature [Pos].
 * @param morphological_parse Current morphological parse object
 * @return "Sing" if the possessive agreement of the parse is singular (contains P1SG, P2SG, P3SG); "Plur" if the
 * possessive agreement of the parse is plural (contains P1PL, P2PL, P3PL).
 */
char *get_possessive_number(const Morphological_parse *morphological_parse) {
    if (parse_contains_tag(morphological_parse, P1SG) || parse_contains_tag(morphological_parse, P2SG) ||
        parse_contains_tag(morphological_parse, P3SG)) {
        return "Sing";
    }
    if (parse_contains_tag(morphological_parse, P1PL) || parse_contains_tag(morphological_parse, P2PL) ||
        parse_contains_tag(morphological_parse, P3PL)) {
        return "Plur";
    }
    return NULL;
}

/**
 * Returns the case marking of the parse for the universal dependency feature case.
 * @param morphological_parse Current morphological parse object
 * @return "Acc" for accusative marker; "Dat" for dative marker; "Gen" for genitive marker; "Loc" for locative
 * marker; "Ins" for instrumentative marker; "Abl" for ablative marker; "Nom" for nominative marker.
 */
char *get_case(const Morphological_parse *morphological_parse) {
    if (parse_contains_tag(morphological_parse, ACCUSATIVE) || parse_contains_tag(morphological_parse, PCACCUSATIVE)) {
        return "Acc";
    }
    if (parse_contains_tag(morphological_parse, DATIVE) || parse_contains_tag(morphological_parse, PCDATIVE)) {
        return "Dat";
    }
    if (parse_contains_tag(morphological_parse, GENITIVE) || parse_contains_tag(morphological_parse, PCGENITIVE)) {
        return "Gen";
    }
    if (parse_contains_tag(morphological_parse, LOCATIVE)) {
        return "Loc";
    }
    if (parse_contains_tag(morphological_parse, INSTRUMENTAL) ||
        parse_contains_tag(morphological_parse, PCINSTRUMENTAL)) {
        return "Ins";
    }
    if (parse_contains_tag(morphological_parse, ABLATIVE) || parse_contains_tag(morphological_parse, PCABLATIVE)) {
        return "Abl";
    }
    if (parse_contains_tag(morphological_parse, EQUATIVE)) {
        return "Equ";
    }
    if (parse_contains_tag(morphological_parse, NOMINATIVE) || parse_contains_tag(morphological_parse, PCNOMINATIVE)) {
        return "Nom";
    }
    return NULL;
}

/**
 * Returns the definiteness of the parse for the universal dependency feature definite. It applies only for
 * determiners in Turkish.
 * @param morphological_parse Current morphological parse object
 * @return "Ind" for 'bir', 'bazı', or 'birkaç'. "Def" for 'her', 'bu', 'şu', 'o', 'bütün'.
 */
char *get_definite(const Morphological_parse *morphological_parse) {
    char *lemma = morphological_parse->root;
    if (parse_contains_tag(morphological_parse, DETERMINER)) {
        if (string_in_list(lemma, (char *[]) {"bir", "bazı", "birkaç", "birçok", "kimi"}, 5)) {
            return "Ind";
        }
        if (string_in_list(lemma, (char *[]) {"her", "bu", "şu", "o", "bütün", "hangi"}, 6)) {
            return "Def";
        }
    }
    return NULL;
}

/**
 * Returns the degree of the parse for the universal dependency feature degree.
 * @param morphological_parse Current morphological parse object
 * @return "Cmp" for comparative adverb 'daha'; "Sup" for superlative adjective or adverb 'en'.
 */
char *get_degree(const Morphological_parse *morphological_parse) {
    char *lemma = morphological_parse->root;
    if (strcmp(lemma, "daha") == 0) {
        return "Cmp";
    }
    if (strcmp(lemma, "en") == 0 && !is_noun(morphological_parse)) {
        return "Sup";
    }
    return NULL;
}

/**
 * Returns the polarity of the verb for the universal dependency feature polarity.
 * @param morphological_parse Current morphological parse object
 * @return "Pos" for positive polarity containing tag POS; "Neg" for negative polarity containing tag NEG.
 */
char *get_polarity(const Morphological_parse *morphological_parse) {
    if (strcmp(morphological_parse->root, "değil") == 0) {
        return "Neg";
    }
    if (parse_contains_tag(morphological_parse, POSITIVE)) {
        return "Pos";
    }
    if (parse_contains_tag(morphological_parse, NEGATIVE)) {
        return "Neg";
    }
    return NULL;
}

/**
 * Returns the person of the agreement of the parse for the universal dependency feature person.
 * @param morphological_parse Current morphological parse object
 * @return "1" for first person; "2" for second person; "3" for third person.
 */
char *get_person(const Morphological_parse *morphological_parse) {
    if (parse_contains_tag(morphological_parse, A1SG) || parse_contains_tag(morphological_parse, A1PL)
        || parse_contains_tag(morphological_parse, P1SG) || parse_contains_tag(morphological_parse, P1PL)) {
        return "1";
    }
    if (parse_contains_tag(morphological_parse, A2SG) || parse_contains_tag(morphological_parse, A2PL)
        || parse_contains_tag(morphological_parse, P2SG) || parse_contains_tag(morphological_parse, P2PL)) {
        return "2";
    }
    if (parse_contains_tag(morphological_parse, A3SG) || parse_contains_tag(morphological_parse, A3PL)
        || parse_contains_tag(morphological_parse, P3SG) || parse_contains_tag(morphological_parse, P3PL)) {
        return "3";
    }
    return NULL;
}

/**
 * Returns the person of the possessive agreement of the parse for the universal dependency feature [pos].
 * @param morphological_parse Current morphological parse object
 * @return "1" for first person; "2" for second person; "3" for third person.
 */
char *get_possessive_person(const Morphological_parse *morphological_parse) {
    if (parse_contains_tag(morphological_parse, P1SG) || parse_contains_tag(morphological_parse, P1PL)) {
        return "1";
    }
    if (parse_contains_tag(morphological_parse, P2SG) || parse_contains_tag(morphological_parse, P2PL)) {
        return "2";
    }
    if (parse_contains_tag(morphological_parse, P3SG) || parse_contains_tag(morphological_parse, P3PL)) {
        return "3";
    }
    return NULL;
}

/**
 * Returns the voice of the verb parse for the universal dependency feature voice.
 * @param morphological_parse Current morphological parse object
 * @return "CauPass" if the verb parse is both causative and passive; "Pass" if the verb parse is only passive;
 * "Rcp" if the verb parse is reciprocal; "Cau" if the verb parse is only causative; "Rfl" if the verb parse is
 * reflexive.
 */
char *get_voice(const Morphological_parse *morphological_parse) {
    if (parse_contains_tag(morphological_parse, CAUSATIVE) && parse_contains_tag(morphological_parse, PASSIVE)) {
        return "CauPass";
    }
    if (parse_contains_tag(morphological_parse, PASSIVE)) {
        return "Pass";
    }
    if (parse_contains_tag(morphological_parse, RECIPROCAL)) {
        return "Rcp";
    }
    if (parse_contains_tag(morphological_parse, CAUSATIVE)) {
        return "Cau";
    }
    if (parse_contains_tag(morphological_parse, REFLEXIVE)) {
        return "Rfl";
    }
    return NULL;
}

/**
 * Returns the aspect of the verb parse for the universal dependency feature aspect.
 * @param morphological_parse Current morphological parse object
 * @return "Perf" for past, narrative and future tenses; "Prog" for progressive tenses; "Hab" for Aorist; "Rapid"
 * for parses containing HASTILY tag; "Dur" for parses containing START, STAY or REPEAT tags.
 */
char *get_aspect(const Morphological_parse *morphological_parse) {
    if (parse_contains_tag(morphological_parse, PASTTENSE) || parse_contains_tag(morphological_parse, NARRATIVE) ||
        parse_contains_tag(morphological_parse, FUTURE)) {
        return "Perf";
    }
    if (parse_contains_tag(morphological_parse, PROGRESSIVE1) ||
        parse_contains_tag(morphological_parse, PROGRESSIVE2)) {
        return "Prog";
    }
    if (parse_contains_tag(morphological_parse, AORIST)) {
        return "Hab";
    }
    if (parse_contains_tag(morphological_parse, HASTILY)) {
        return "Rapid";
    }
    if (parse_contains_tag(morphological_parse, START) || parse_contains_tag(morphological_parse, STAY) ||
        parse_contains_tag(morphological_parse, REPEAT)) {
        return "Dur";
    }
    return NULL;
}

/**
 * Returns the tense of the verb parse for universal dependency feature tense.
 * @param morphological_parse Current morphological parse object
 * @return "Past" for simple past tense; "Fut" for future tense; "Pqp" for narrative past tense; "Pres" for other
 * past tenses.
 */
char *get_tense(const Morphological_parse *morphological_parse) {
    if (parse_contains_tag(morphological_parse, NARRATIVE) && parse_contains_tag(morphological_parse, PASTTENSE)) {
        return "Pqp";
    }
    if (parse_contains_tag(morphological_parse, NARRATIVE) || parse_contains_tag(morphological_parse, PASTTENSE)) {
        return "Past";
    }
    if (parse_contains_tag(morphological_parse, FUTURE)) {
        return "Fut";
    }
    if (!parse_contains_tag(morphological_parse, PASTTENSE) && !parse_contains_tag(morphological_parse, FUTURE)) {
        return "Pres";
    }
    return NULL;
}

/**
 * Returns the modality of the verb parse for the universal dependency feature mood.
 * @return "GenNecPot" if both necessitative and potential is combined with a suffix of general modality;
 * "CndGenPot" if both conditional and potential is combined with a suffix of general modality;
 * "GenNec" if necessitative is combined with a suffix of general modality;
 * "GenPot" if potential is combined with a suffix of general modality;
 * "NecPot" if necessitative is combined with potential;
 * "DesPot" if desiderative is combined with potential;
 * "CndPot" if conditional is combined with potential;
 * "CndGen" if conditional is combined with a suffix of general modality;
 * "Imp" for imperative; "Cnd" for simple conditional; "Des" for simple desiderative; "Opt" for optative; "Nec" for
 * simple necessitative; "Pot" for simple potential; "Gen" for simple suffix of a general modality.
 * @param morphological_parse Current morphological parse object
 */
char *get_mood(const Morphological_parse *morphological_parse) {
    if ((parse_contains_tag(morphological_parse, COPULA) || parse_contains_tag(morphological_parse, AORIST)) && parse_contains_tag(morphological_parse, NECESSITY) && parse_contains_tag(morphological_parse, ABLE)){
        return "GenNecPot";
    }
    if ((parse_contains_tag(morphological_parse, COPULA) || parse_contains_tag(morphological_parse, AORIST)) && parse_contains_tag(morphological_parse, CONDITIONAL) && parse_contains_tag(morphological_parse, ABLE)){
        return "CndGenPot";
    }
    if ((parse_contains_tag(morphological_parse, COPULA) || parse_contains_tag(morphological_parse, AORIST)) && parse_contains_tag(morphological_parse, NECESSITY)){
        return "GenNec";
    }
    if ((parse_contains_tag(morphological_parse, COPULA) || parse_contains_tag(morphological_parse, AORIST)) && parse_contains_tag(morphological_parse, ABLE)){
        return "GenPot";
    }
    if (parse_contains_tag(morphological_parse, NECESSITY) && parse_contains_tag(morphological_parse, ABLE)){
        return "NecPot";
    }
    if (parse_contains_tag(morphological_parse, DESIRE) && parse_contains_tag(morphological_parse, ABLE)){
        return "DesPot";
    }
    if (parse_contains_tag(morphological_parse, CONDITIONAL) && parse_contains_tag(morphological_parse, ABLE)){
        return "CndPot";
    }
    if (parse_contains_tag(morphological_parse, CONDITIONAL) && (parse_contains_tag(morphological_parse, COPULA) || parse_contains_tag(morphological_parse, AORIST))){
        return "CndGen";
    }
    if (parse_contains_tag(morphological_parse, IMPERATIVE)) {
        return "Imp";
    }
    if (parse_contains_tag(morphological_parse, CONDITIONAL)) {
        return "Cnd";
    }
    if (parse_contains_tag(morphological_parse, DESIRE)) {
        return "Des";
    }
    if (parse_contains_tag(morphological_parse, OPTATIVE)) {
        return "Opt";
    }
    if (parse_contains_tag(morphological_parse, NECESSITY)) {
        return "Nec";
    }
    if (parse_contains_tag(morphological_parse, ABLE)) {
        return "Pot";
    }
    if (parse_contains_tag(morphological_parse, PASTTENSE) || parse_contains_tag(morphological_parse, NARRATIVE) || parse_contains_tag(morphological_parse, PROGRESSIVE1) || parse_contains_tag(morphological_parse, PROGRESSIVE2) || parse_contains_tag(morphological_parse, FUTURE)){
        return "Ind";
    }
    if ((parse_contains_tag(morphological_parse, COPULA) || parse_contains_tag(morphological_parse, AORIST))){
        return "Gen";
    }
    if (parse_contains_tag(morphological_parse, ZERO) && !parse_contains_tag(morphological_parse, A3PL)){
        return "Gen";
    }
    return NULL;
}

/**
 * Returns the form of the verb parse for the universal dependency feature verbForm.
 * @param morphological_parse Current morphological parse object
 * @return "Part" for participles; "Vnoun" for infinitives; "Conv" for parses contaning tags SINCEDOINGSO,
 * WITHOUTHAVINGDONESO, WITHOUTBEINGABLETOHAVEDONESO, BYDOINGSO, AFTERDOINGSO, INFINITIVE3; "Fin" for others.
 */
char *get_verb_form(const Morphological_parse *morphological_parse) {
    if (parse_contains_tag(morphological_parse, PASTPARTICIPLE) ||
        parse_contains_tag(morphological_parse, FUTUREPARTICIPLE) ||
        parse_contains_tag(morphological_parse, PRESENTPARTICIPLE)) {
        return "Part";
    }
    if (parse_contains_tag(morphological_parse, INFINITIVE) || parse_contains_tag(morphological_parse, INFINITIVE2)){
        return "Vnoun";
    }
    if (parse_contains_tag(morphological_parse, SINCEDOINGSO) || parse_contains_tag(morphological_parse, WITHOUTHAVINGDONESO) || parse_contains_tag(morphological_parse, WITHOUTBEINGABLETOHAVEDONESO) || parse_contains_tag(morphological_parse, BYDOINGSO) || parse_contains_tag(morphological_parse, AFTERDOINGSO) || parse_contains_tag(morphological_parse, INFINITIVE3)){
        return "Conv";
    }
    if (parse_contains_tag(morphological_parse, COPULA) || parse_contains_tag(morphological_parse, ABLE) || parse_contains_tag(morphological_parse, AORIST) || parse_contains_tag(morphological_parse, PROGRESSIVE2)
        || parse_contains_tag(morphological_parse, DESIRE) || parse_contains_tag(morphological_parse, NECESSITY) || parse_contains_tag(morphological_parse, CONDITIONAL) || parse_contains_tag(morphological_parse, IMPERATIVE) || parse_contains_tag(morphological_parse, OPTATIVE)
        || parse_contains_tag(morphological_parse, PASTTENSE) || parse_contains_tag(morphological_parse, NARRATIVE) || parse_contains_tag(morphological_parse, PROGRESSIVE1) || parse_contains_tag(morphological_parse, FUTURE)
        || (parse_contains_tag(morphological_parse, ZERO) && !parse_contains_tag(morphological_parse, A3PL))){
        return "Fin";
    }
    return NULL;
}

char *get_evident(const Morphological_parse *morphological_parse) {
    if (parse_contains_tag(morphological_parse, NARRATIVE)) {
        return "Nfh";
    } else {
        if (parse_contains_tag(morphological_parse, COPULA) ||
            parse_contains_tag(morphological_parse, ABLE) ||
            parse_contains_tag(morphological_parse, AORIST) ||
            parse_contains_tag(morphological_parse, PROGRESSIVE2) ||
            parse_contains_tag(morphological_parse, DESIRE) ||
            parse_contains_tag(morphological_parse, NECESSITY) ||
            parse_contains_tag(morphological_parse, CONDITIONAL) ||
            parse_contains_tag(morphological_parse, IMPERATIVE) ||
            parse_contains_tag(morphological_parse, OPTATIVE) ||
            parse_contains_tag(morphological_parse, PASTTENSE) ||
            parse_contains_tag(morphological_parse, NARRATIVE) ||
            parse_contains_tag(morphological_parse, PROGRESSIVE1) ||
            parse_contains_tag(morphological_parse, FUTURE)) {
            return "Fh";
        }
    }
    return NULL;
}

/**
 * Construct the universal dependency features as an array of strings. Each element represents a single feature.
 * Every feature is given as featureType = featureValue.
 * @param morphological_parse Current morphological parse object
 * @param u_pos Universal dependency part of speech tag for the parse.
 * @return An array of universal dependency features for this parse.
 */
Array_list_ptr get_universal_dependency_features(const Morphological_parse *morphological_parse, const char *u_pos) {
    Array_list_ptr feature_list = create_array_list();
    char *pron_type = get_pron_type(morphological_parse);
    if (!pron_type && strcmp(u_pos, "NOUN") != 0 && strcmp(u_pos, "ADJ") != 0 && strcmp(u_pos, "VERB") != 0 && strcmp(u_pos, "CCONJ") != 0 && strcmp(u_pos, "PROPN") != 0) {
        array_list_add(feature_list, str_concat("PronType=", pron_type));
    }
    char *num_type = get_num_type(morphological_parse);
    if (num_type != NULL && strcmp(u_pos, "VERB") != 0 && strcmp(u_pos, "NOUN") != 0 && strcmp(u_pos, "ADV") != 0) {
        array_list_add(feature_list, str_concat("NumType=", num_type));
    }
    char *reflex = get_reflex(morphological_parse);
    if (reflex != NULL && strcmp(u_pos, "VERB") != 0 && strcmp(u_pos, "ADJ") != 0) {
        array_list_add(feature_list, str_concat("Reflex=", reflex));
    }
    char *degree = get_degree(morphological_parse);
    if (degree != NULL && strcmp(u_pos, "ADJ") != 0) {
        array_list_add(feature_list, str_concat("Degree=", degree));
    }
    if (is_noun(morphological_parse) || is_parse_verb(morphological_parse) || strcmp(morphological_parse->root, "mi") == 0 || (pron_type != NULL && strcmp(pron_type, "Art") != 0)) {
        char *number = get_number(morphological_parse);
        if (number != NULL) {
            array_list_add(feature_list, str_concat("Number=", number));
        }
        char *possessive_number = get_possessive_number(morphological_parse);
        if (possessive_number != NULL) {
            array_list_add(feature_list, str_concat("Number[psor]=", possessive_number));
        }
        char *person = get_person(morphological_parse);
        if (person != NULL && strcmp(u_pos, "PROPN") != 0) {
            array_list_add(feature_list, str_concat("Person=", person));
        }
        char *possessive_person = get_possessive_person(morphological_parse);
        if (possessive_person != NULL && strcmp(u_pos, "PROPN") != 0) {
            array_list_add(feature_list, str_concat("Person[psor]=", possessive_person));
        }
    }
    if (is_noun(morphological_parse) || (pron_type != NULL && strcmp(pron_type, "Art") != 0)) {
        char *case_ = get_case(morphological_parse);
        if (case_ != NULL) {
            array_list_add(feature_list, str_concat("Case=", case_));
        }
    }
    if (parse_contains_tag(morphological_parse, DETERMINER)) {
        char *definite = get_definite(morphological_parse);
        if (definite != NULL) {
            array_list_add(feature_list, str_concat("Definite=", definite));
        }
    }
    if (is_parse_verb(morphological_parse) || strcmp(morphological_parse->root, "mi") == 0) {
        char *polarity = get_polarity(morphological_parse);
        if (polarity != NULL) {
            array_list_add(feature_list, str_concat("Polarity=", polarity));
        }
        char *voice = get_voice(morphological_parse);
        if (voice != NULL && strcmp(morphological_parse->root, "mi") != 0) {
            array_list_add(feature_list, str_concat("Voice=", voice));
        }
        char *aspect = get_aspect(morphological_parse);
        if (aspect != NULL && strcmp(u_pos, "PROPN") != 0 && strcmp(morphological_parse->root, "mi") != 0) {
            array_list_add(feature_list, str_concat("Aspect=", aspect));
        }
        char *tense = get_tense(morphological_parse);
        if (tense != NULL && strcmp(u_pos, "PROPN") != 0) {
            array_list_add(feature_list, str_concat("Tense=", tense));
        }
        char *mood = get_mood(morphological_parse);
        if (mood != NULL && strcmp(u_pos, "PROPN") != 0 && strcmp(morphological_parse->root, "mi") != 0) {
            array_list_add(feature_list, str_concat("Mood=", mood));
        }
        char *verbForm = get_verb_form(morphological_parse);
        if (verbForm != NULL) {
            array_list_add(feature_list, str_concat("VerbForm=", verbForm));
        }
        char *evident = get_evident(morphological_parse);
        if (evident != NULL && strcmp(morphological_parse->root, "mi") != 0) {
            array_list_add(feature_list, str_concat("Evident=", evident));
        }
    }
    array_list_sort(feature_list, (int (*)(const void *, const void *)) compare_string);
    return feature_list;
}

/**
 * Returns the universal dependency part of speech for this parse.
 * @param morphological_parse Current morphological parse object
 * @return "AUX" for word 'değil; "PROPN" for proper nouns; "NOUN for nouns; "ADJ" for adjectives; "ADV" for
 * adverbs; "INTJ" for interjections; "VERB" for verbs; "PUNCT" for punctuation symbols; "DET" for determiners;
 * "NUM" for numerals; "PRON" for pronouns; "ADP" for post participles; "SCONJ" or "CCONJ" for conjunctions.
 */
char *get_universal_dependency_pos(const Morphological_parse *morphological_parse) {
    char *lemma = morphological_parse->root;
    if (strcmp(lemma, "değil") == 0) {
        return "AUX";
    }
    if (is_parse_proper_noun(morphological_parse)) {
        return "PROPN";
    }
    if (is_noun(morphological_parse)) {
        return "NOUN";
    }
    if (is_parse_adjective(morphological_parse)) {
        return "ADJ";
    }
    if (parse_contains_tag(morphological_parse, INTERJECTION)) {
        return "INTJ";
    }
    if (is_parse_verb(morphological_parse)) {
        return "VERB";
    }
    if (is_parse_punctuation(morphological_parse) || is_hash_tag(morphological_parse)) {
        return "PUNCT";
    }
    if (parse_contains_tag(morphological_parse, DETERMINER)) {
        return "DET";
    }
    if (is_number(morphological_parse) || is_parse_date(morphological_parse) || is_parse_time(morphological_parse) ||
        is_parse_ordinal(morphological_parse) ||
        is_parse_fraction(morphological_parse) || strcmp(lemma, "%") == 0) {
        return "NUM";
    }
    char *pos = get_pos(morphological_parse);
    if (strcmp(pos, "ADV") == 0) {
        free_(pos);
        return "ADV";
    }
    if (strcmp(pos, "PRON") == 0) {
        free_(pos);
        return "PRON";
    }
    if (strcmp(pos, "POSTP") == 0) {
        free_(pos);
        return "ADP";
    }
    if (strcmp(pos, "QUES") == 0) {
        free_(pos);
        return "AUX";
    }
    if (strcmp(pos, "CONJ") == 0) {
        free_(pos);
        if (string_in_list(lemma, (char *[]) {"ki", "eğer", "diye"}, 3)) {
            return "SCONJ";
        } else {
            return "CCONJ";
        }
    }
    free_(pos);
    return "X";
}

/**
 * The overridden toString method gets the root and the first inflectional group as a result {@link String} then concatenates
 * with ^DB+ and the following inflectional groups.
 *
 * @param morphological_parse Current morphological parse object
 * @return result {@link String}.
 */
char *morphological_parse_to_string(const Morphological_parse *morphological_parse) {
    char tmp[MAX_LINE_LENGTH];
    char *st = inflectional_group_to_string(array_list_get(morphological_parse->inflectional_groups, 0));
    sprintf(tmp, "%s+%s", morphological_parse->root, st);
    free_(st);
    for (int i = 1; i < morphological_parse->inflectional_groups->size; i++) {
        st = inflectional_group_to_string(array_list_get(morphological_parse->inflectional_groups, i));
        sprintf(tmp, "%s^DB+%s", tmp, st);
        free_(st);
    }
    char *result = NULL;
    result = str_copy(result, tmp);
    return result;
}

/**
 * Frees memory allocated to a morphological parse. Deallocates the array of inflectional groups.
 * @param morphological_parse Morphological parse to be deallocated.
 */
void free_morphological_parse(Morphological_parse_ptr morphological_parse) {
    free_array_list(morphological_parse->inflectional_groups, (void (*)(void *)) free_inflectional_group);
    free_(morphological_parse->root);
    free_(morphological_parse);
}
