//
// Created by Olcay Taner YILDIZ on 18.10.2023.
//

#include <Corpus.h>
#include "../src/DisambiguationCorpus.h"

int main(){
    Corpus_ptr corpus = create_disambiguation_corpus("penntreebank.txt");
    if (corpus->sentences->size != 19108){
        printf("Sentence count wrong! %d\n", corpus->sentences->size);
    }
    if (corpus_number_of_words(corpus) != 170211){
        printf("Word count wrong! %d\n", corpus_number_of_words(corpus));
    }
}