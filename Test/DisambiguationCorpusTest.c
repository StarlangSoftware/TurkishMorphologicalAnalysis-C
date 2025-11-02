//
// Created by Olcay Taner YILDIZ on 18.10.2023.
//

#include <Corpus.h>
#include <Memory/Memory.h>

#include "../src/DisambiguationCorpus.h"

int main(){
    start_x_large_memory_check();
    Corpus_ptr corpus = create_disambiguation_corpus("etstur.txt");
    if (corpus->sentences->size != 19108){
        printf("Sentence count wrong! %d\n", corpus->sentences->size);
    }
    if (corpus_number_of_words(corpus) != 170211){
        printf("Word count wrong! %d\n", corpus_number_of_words(corpus));
    }
    free_disambiguation_corpus(corpus);
    end_memory_check();
}