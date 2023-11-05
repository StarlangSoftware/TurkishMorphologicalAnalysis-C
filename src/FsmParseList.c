//
// Created by Olcay Taner YILDIZ on 24.10.2023.
//

#include <string.h>
#include <stdlib.h>
#include <FileUtils.h>
#include "FsmParseList.h"
#include "FsmParse.h"
#include "MorphologicalParse.h"

/**
 * A constructor of FsmParseList class which takes an vector fsmParses as an input. First it sorts
 * the items of the vector then loops through it, if the current item's transitions equal to the next item's
 * transitions, it removes the latter item. At the end, it assigns this vector to the fsmParses variable.
 *
 * @param fsmParses FsmParse type vector input.
 */
Fsm_parse_list_ptr create_fsm_parse_list(Array_list_ptr fsmParses) {
    Fsm_parse_list_ptr result = malloc(sizeof(Fsm_parse_list));
    if (fsmParses->size > 0){
        array_list_merge_sort(fsmParses, (int (*)(const void *, const void *)) compare_fsm_parse);
        for (int i = 0; i < fsmParses->size - 1; i++) {
            char* transition1 = transition_list(array_list_get(fsmParses, i));
            char* transition2 = transition_list(array_list_get(fsmParses, i + 1));
            if (strcmp(transition1, transition2) == 0) {
                array_list_remove(fsmParses, i + 1, (void (*)(void *)) free_fsm_parse);
                i--;
            }
            free(transition1);
            free(transition2);
        }
    }
    result->fsm_parses = fsmParses;
    return result;
}

void free_fsm_parse_list(Fsm_parse_list_ptr fsm_parse_list) {
    free_array_list(fsm_parse_list->fsm_parses, (void (*)(void *)) free_fsm_parse);
    free(fsm_parse_list);
}

/**
 * The rootWords method gets the first item's root of fsmParses vector and uses it as currentRoot. Then loops through
 * the fsmParses, if the current item's root does not equal to the currentRoot, it then assigns it as the currentRoot and
 * accumulates root words in a String result.
 *
 * @return String result that has root words.
 */
char *root_words(Fsm_parse_list_ptr fsm_parse_list) {
    char tmp[MAX_LINE_LENGTH];
    char* currentRoot = (get_fsm_parse(fsm_parse_list, 0))->root->word->name;
    sprintf(tmp, "%s", currentRoot);
    for (int i = 1; i < fsm_parse_list->fsm_parses->size; i++) {
        char* name = (get_fsm_parse(fsm_parse_list, i))->root->word->name;
        if (strcmp(name, currentRoot) != 0) {
            currentRoot = name;
            sprintf(tmp, "%s$%s", tmp, currentRoot);
        }
    }
    char *result = NULL;
    result = str_copy(result, tmp);
    return result;
}

/**
 * The reduceToParsesWithSameRootAndPos method takes a Word currentWithPos as an input and loops i times till
 * i equals to the size of the fsmParses vector. If the given currentWithPos does not equal to the ith item's
 * root and the MorphologicalTag of the first inflectional of fsmParses, it removes the ith item from the vector.
 *
 * @param currentWithPos Word input.
 */
void reduce_to_parses_with_same_root_and_pos(Fsm_parse_list_ptr fsm_parse_list, Txt_word_ptr current_with_pos) {
    int i = 0;
    while (i < fsm_parse_list->fsm_parses->size) {
        char* name = get_word_with_pos2(get_fsm_parse(fsm_parse_list, i))->name;
        if (strcmp(name, current_with_pos->word->name) != 0) {
            array_list_remove(fsm_parse_list->fsm_parses, i, (void (*)(void *)) free_fsm_parse);
        } else {
            i++;
        }
    }
}

/**
 * The getParseWithLongestRootWord method returns the parse with the longest root word. If more than one parse has the
 * longest root word, the first parse with that root is returned. If the longest root word belongs to an exceptional
 * case, the parse with the next longest root word that does not, is returned.
 *
 * @return FsmParse Parse with the longest root word.
 */
Fsm_parse_ptr get_parse_with_longest_root_word(Fsm_parse_list_ptr fsm_parse_list) {
    Fsm_parse_ptr bestParse;
    int maxLength = -1;
    if (fsm_parse_list->fsm_parses->size > 0){
        bestParse = get_fsm_parse(fsm_parse_list, 0);
    }
    for (int i = 0; i < fsm_parse_list->fsm_parses->size; i++){
        Fsm_parse_ptr fsmParse = get_fsm_parse(fsm_parse_list, i);
        int length = word_size(fsmParse->root->word->name);
        if (length > maxLength && !is_longest_root_exception(fsm_parse_list, fsmParse)){
            maxLength = length;
            bestParse = fsmParse;
        }
    }
    return bestParse;
}

/**
 * The isLongestRootException method returns true if the longest root word belongs to an exceptional case, false otherwise.
 *
 * @param fsmParse FsmParse input.
 * @return true if the longest root belongs to an exceptional case, false otherwise.
 */
bool is_longest_root_exception(Fsm_parse_list_ptr fsm_parse_list, Fsm_parse_ptr fsm_parse) {
    char* surfaceForm = fsm_parse->form;
    char* root = fsm_parse->root->word->name;
    for (int i = 0; i < 231; i++) {
        char* longestRootException = longestRootExceptions[i];
        Array_list_ptr exceptionItems = str_split(longestRootException, ' ');
        char* surfaceFormEnding = array_list_get(exceptionItems, 0);
        char* longestRootEnding = array_list_get(exceptionItems, 1);
        char* longestRootPos = array_list_get(exceptionItems, 2);
        char* possibleRootPos = array_list_get(exceptionItems, 3);
        char* possibleRoot = replace_all(surfaceForm, surfaceFormEnding, "");
        char* rootPos1 = get_tag(get_tag_with_index(first_inflectional_group2(fsm_parse), 0));
        if (ends_with(surfaceForm, surfaceFormEnding) && ends_with(root, longestRootEnding) && strcmp(rootPos1, longestRootPos) == 0) {
            free(rootPos1);
            for (int j = 0; j < fsm_parse_list->fsm_parses->size; j++) {
                Fsm_parse_ptr currentParse = get_fsm_parse(fsm_parse_list, j);
                char* rootPos2 = get_tag(get_tag_with_index(first_inflectional_group2(currentParse), 0));
                if (strcmp(currentParse->root->word->name, possibleRoot) == 0 && strcmp(rootPos2, possibleRootPos) == 0) {
                    free(rootPos2);
                    return true;
                }
                free(rootPos2);
            }
        } else {
            free(rootPos1);
        }
    }
    return false;
}

/**
 * The reduceToParsesWithSameRoot method takes a String currentWithPos as an input and loops i times till
 * i equals to the size of the fsmParses vector. If the given currentRoot does not equal to the root of ith item of
 * fsmParses, it removes the ith item from the vector.
 *
 * @param currentRoot String input.
 */
void reduce_to_parses_with_same_root(Fsm_parse_list_ptr fsm_parse_list, char *currentRoot) {
    int i = 0;
    while (i < fsm_parse_list->fsm_parses->size) {
        Fsm_parse_ptr fsm_parse = get_fsm_parse(fsm_parse_list, i);
        if (strcmp(fsm_parse->root->word->name, currentRoot) != 0) {
            array_list_remove(fsm_parse_list->fsm_parses, i, (void (*)(void *)) free_fsm_parse);
        } else {
            i++;
        }
    }
}

/**
 * The constructParseListForDifferentRootWithPos method initially creates a result ArrayList then loops through the
 * fsmParses ArrayList. For the first iteration, it creates new ArrayList as initial, then adds the
 * first item od fsmParses to initial and also add this initial ArrayList to the result ArrayList.
 * For the following iterations, it checks whether the current item's root with the MorphologicalTag of the first inflectional
 * equal to the previous item's  root with the MorphologicalTag of the first inflectional. If so, it adds that item
 * to the result ArrayList, if not it creates new ArrayList as initial and adds the first item od fsmParses
 * to initial and also add this initial ArrayList to the result ArrayList.
 *
 * @return result ArrayList type of FsmParseList.
 */
Array_list_ptr construct_parse_list_for_different_root_with_pos(Fsm_parse_list_ptr fsm_parse_list) {
    Array_list_ptr result = create_array_list();
    int i = 0;
    while (i < fsm_parse_list->fsm_parses->size) {
        if (i == 0) {
            Array_list_ptr initial = create_array_list();
            array_list_add(initial, get_fsm_parse(fsm_parse_list, i));
            array_list_add(result, create_fsm_parse_list(initial));
        } else {
            Word_ptr word1 = get_word_with_pos2(get_fsm_parse(fsm_parse_list, i));
            Word_ptr word2 = get_word_with_pos2(get_fsm_parse(fsm_parse_list, i - 1));
            if (strcmp(word1->name, word2->name) == 0) {
                array_list_add(((Fsm_parse_list_ptr)array_list_get(result, result->size - 1))->fsm_parses,
                               get_fsm_parse(fsm_parse_list, i));
            } else {
                Array_list_ptr initial = create_array_list();
                array_list_add(initial, get_fsm_parse(fsm_parse_list, i));
                array_list_add(result, create_fsm_parse_list(initial));
            }
            free_word(word1);
            free_word(word2);
        }
        i++;
    }
    return result;
}

/**
 * The overridden toString method loops through the fsmParses ArrayList and accumulates the items to a result String.
 *
 * @return result String that has the items of fsmParses ArrayList.
 */
char *fsm_parse_list_to_string(Fsm_parse_list_ptr fsm_parse_list) {
    char tmp[MAX_LINE_LENGTH] = "";
    for (int i = 0; i < fsm_parse_list->fsm_parses->size; i++) {
        Fsm_parse_ptr fsm_parse = get_fsm_parse(fsm_parse_list, i);
        sprintf(tmp, "%s%s\n", tmp, fsm_parse_to_string(fsm_parse));
    }
    char *result = NULL;
    result = str_copy(result, tmp);
    return result;
}

/**
 * The parsesWithoutPrefixAndSuffix method first creates a String array named analyses with the size of fsmParses ArrayList's size.
 * <p>
 * If the size is just 1, it then returns the first item's transitionList, if it is greater than 1, loops through the fsmParses and
 * puts the transitionList of each item to the analyses array.
 * <p>
 * If the removePrefix condition holds, it loops through the analyses array and takes each item's substring after the first + sign and updates that
 * item of analyses array with that substring.
 * <p>
 * If the removeSuffix condition holds, it loops through the analyses array and takes each item's substring till the last + sign and updates that
 * item of analyses array with that substring.
 * <p>
 * It then removes the duplicate items of analyses array and returns a result String that has the accumulated items of analyses array.
 *
 * @return result String that has the accumulated items of analyses array.
 */
char *parses_without_prefix_and_suffix(Fsm_parse_list_ptr fsm_parse_list) {
    char tmp[MAX_LINE_LENGTH] = "";
    char* analyses[fsm_parse_list->fsm_parses->size];
    bool removePrefix = true, removeSuffix = true;
    char* list = transition_list(get_fsm_parse(fsm_parse_list, 0));
    if (fsm_parse_list->fsm_parses->size == 1) {
        String_ptr st = substring2(list, str_find_c(list, "+") + 1);
        char* result = str_copy(result, st->s);
        free_string_ptr(st);
        return result;
    }
    for (int i = 0; i < fsm_parse_list->fsm_parses->size; i++) {
        analyses[i] = transition_list(get_fsm_parse(fsm_parse_list, i));
    }
    while (removePrefix) {
        removePrefix = true;
        for (int i = 0; i < fsm_parse_list->fsm_parses->size - 1; i++) {
            if (str_contains_c(analyses[i], '+') && str_contains_c(analyses[i + 1], '+')){
                int index1 = str_find_c(analyses[i], "+") + 1;
                int index2 = str_find_c(analyses[i + 1], "+") + 1;
                String_ptr st1 = substring(analyses[i], 0, index1);
                String_ptr st2 = substring(analyses[i + 1], 0, index2);
                if (!string_equals(st1, st2)) {
                    free_string_ptr(st1);
                    free_string_ptr(st2);
                    removePrefix = false;
                    break;
                }
                free_string_ptr(st1);
                free_string_ptr(st2);
            } else {
                removePrefix = false;
                break;
            }
        }
        if (removePrefix) {
            for (int i = 0; i < fsm_parse_list->fsm_parses->size; i++) {
                String_ptr st1 = substring2(analyses[i], str_find_c(analyses[i], "+") + 1);
                free(analyses[i]);
                analyses[i] = str_copy(analyses[i], st1->s);
                free_string_ptr(st1);
            }
        }
    }
    while (removeSuffix) {
        removeSuffix = true;
        for (int i = 0; i < fsm_parse_list->fsm_parses->size - 1; i++) {
            if (str_contains_c(analyses[i], '+') && str_contains_c(analyses[i + 1], '+')){
                int index1 = str_find_last_c(analyses[i], "+");
                int index2 = str_find_last_c(analyses[i + 1], "+");
                String_ptr st1 = substring2(analyses[i], index1);
                String_ptr st2 = substring2(analyses[i + 1], index2);
                if (!string_equals(st1, st2)) {
                    removeSuffix = false;
                    free_string_ptr(st1);
                    free_string_ptr(st2);
                    break;
                }
                free_string_ptr(st1);
                free_string_ptr(st2);
            } else {
                removeSuffix = false;
                break;
            }
        }
        if (removeSuffix) {
            for (int i = 0; i < fsm_parse_list->fsm_parses->size; i++) {
                String_ptr st1 = substring(analyses[i], 0, str_find_last_c(analyses[i], "+"));
                free(analyses[i]);
                analyses[i] = str_copy(analyses[i], st1->s);
                free_string_ptr(st1);
            }
        }
    }
    for (int i = 0; i < fsm_parse_list->fsm_parses->size; i++) {
        for (int j = i + 1; j < fsm_parse_list->fsm_parses->size; j++) {
            if (compare_string(analyses[i], analyses[j]) > 0) {
                char* tmp2 = analyses[i];
                analyses[i] = analyses[j];
                analyses[j] = tmp2;
            }
        }
    }
    sprintf(tmp, "%s", analyses[0]);
    for (int i = 1; i < fsm_parse_list->fsm_parses->size; i++) {
        sprintf(tmp, "%s$%s", tmp, analyses[i]);
    }
    char *result = NULL;
    result = str_copy(result, tmp);
    return result;
}

Fsm_parse_list_ptr create_fsm_parse_list2() {
    Fsm_parse_list_ptr result = malloc(sizeof(Fsm_parse_list));
    result->fsm_parses = create_array_list();
    return result;
}

Fsm_parse_ptr get_fsm_parse(Fsm_parse_list_ptr fsm_parse_list, int index) {
    return array_list_get(fsm_parse_list->fsm_parses, index);
}

Fsm_parse_list_ptr clone_fsm_parse_list(Fsm_parse_list_ptr fsm_parse_list) {
    Fsm_parse_list_ptr result = malloc(sizeof(Fsm_parse_list));
    result->fsm_parses = create_array_list();
    for (int i = 0; i < fsm_parse_list->fsm_parses->size; i++){
        Fsm_parse_ptr fsm_parse = array_list_get(fsm_parse_list->fsm_parses, i);
        array_list_add(result->fsm_parses, clone_fsm_parse(fsm_parse));
    }
    return result;
}
