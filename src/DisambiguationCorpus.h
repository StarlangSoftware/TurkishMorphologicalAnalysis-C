//
// Created by Olcay Taner YILDIZ on 18.10.2023.
//

#ifndef MORPHOLOGICALANALYSIS_DISAMBIGUATIONCORPUS_H
#define MORPHOLOGICALANALYSIS_DISAMBIGUATIONCORPUS_H

#include <Corpus.h>

Corpus_ptr create_disambiguation_corpus(const char* file_name);

void free_disambiguation_sentence(Sentence_ptr sentence);

void free_disambiguation_corpus(Corpus_ptr corpus);

#endif //MORPHOLOGICALANALYSIS_DISAMBIGUATIONCORPUS_H
