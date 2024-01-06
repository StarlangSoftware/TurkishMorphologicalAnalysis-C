cmake_minimum_required(VERSION 3.23)
project(MorphologicalAnalysis)
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_FLAGS "-O3")

find_package(xml_parser_c REQUIRED)
find_package(corpus_c REQUIRED)

add_library(MorphologicalAnalysis MorphologicalTag.h FsmState.c FsmState.h InflectionalGroup.c InflectionalGroup.h MetamorphicParse.c MetamorphicParse.h MorphologicalParse.c MorphologicalParse.h DisambiguatedWord.c DisambiguatedWord.h DisambiguationCorpus.c DisambiguationCorpus.h FiniteStateMachine.c FiniteStateMachine.h MorphotacticEngine.c MorphotacticEngine.h Transition.c Transition.h FsmParse.c FsmParse.h FsmParseList.c FsmParseList.h FsmMorphologicalAnalyzer.c FsmMorphologicalAnalyzer.h)
target_link_libraries(MorphologicalAnalysis xml_parser_c::xml_parser_c corpus_c::corpus_c)
