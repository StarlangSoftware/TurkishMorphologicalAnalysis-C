//
// Created by Olcay Taner YILDIZ on 18.10.2023.
//

#include <FileUtils.h>
#include <string.h>
#include <Memory/Memory.h>
#include "DisambiguationCorpus.h"
#include "DisambiguatedWord.h"

/**
 * Constructor which takes a file name String as an input and reads the file line by line. It takes each word of the line,
 * and creates a new DisambiguatedWord with current word and its MorphologicalParse. It also creates a new Sentence
 * when a new sentence starts, and adds each word to this sentence till the end of that sentence.
 *
 * @param file_name File which will be read and parsed.
 */
Corpus_ptr create_disambiguation_corpus(const char *file_name) {
    Disambiguated_word_ptr new_word;
    Sentence_ptr new_sentence = NULL;
    Corpus_ptr result = create_corpus();
    result->file_name = str_copy(result->file_name, file_name);
    Array_list_ptr lines = read_lines(file_name);
    for (int i = 0; i < lines->size; i++){
        char* line = array_list_get(lines, i);
        Array_list_ptr tokens = str_split(line, '\t');
        char* word = array_list_get(tokens, 0);
        char* parse = array_list_get(tokens, 1);
        if (word != NULL && parse != NULL){
            new_word = create_disambiguated_word(word, create_morphological_parse(parse));
            if (strcmp(word, "<S>") == 0) {
                new_sentence = create_sentence();
            } else {
                if (strcmp(word, "</S>") == 0) {
                    corpus_add_sentence(result, new_sentence);
                } else {
                    if (string_in_list(word, (char*[]) {"<DOC>", "</DOC>", "<TITLE>", "</TITLE>"}, 4)) {
                    } else {
                        if (new_sentence != NULL) {
                            array_list_add(new_sentence->words, new_word);
                        }
                    }
                }
            }
        }
        free_array_list(tokens, free_);
    }
    free_array_list(lines, free_);
    return result;
}

/**
 * Frees memory allocated for a sentence containing disambiguated words. Frees word array.
 * @param sentence Sentence to be deallocated.
 */
void free_disambiguation_sentence(Sentence_ptr sentence) {
    free_array_list(sentence->words, (void (*)(void *)) free_disambiguated_word);
    free_(sentence);
}

/**
 * Frees memory allocated for a disambiguation corpus. Frees sentences, paragraphs and word list.
 * @param corpus Corpus to be deallocated.
 */
void free_disambiguation_corpus(Corpus_ptr corpus) {
    free_array_list(corpus->sentences, (void (*)(void *)) free_disambiguation_sentence);
    free_array_list(corpus->paragraphs, NULL);
    free_counter_hash_map(corpus->word_list);
    free_(corpus);
}
