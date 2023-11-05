cmake_minimum_required(VERSION 3.23)
project(MorphologicalAnalysis C)

set(CMAKE_C_STANDARD 17)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_library(MorphologicalAnalysis src/MorphologicalTag.h src/FsmState.c src/FsmState.h src/InflectionalGroup.c src/InflectionalGroup.h src/MetamorphicParse.c src/MetamorphicParse.h src/MorphologicalParse.c src/MorphologicalParse.h src/DisambiguatedWord.c src/DisambiguatedWord.h src/DisambiguationCorpus.c src/DisambiguationCorpus.h src/FiniteStateMachine.c src/FiniteStateMachine.h src/MorphotacticEngine.c src/MorphotacticEngine.h src/Transition.c src/Transition.h src/FsmParse.c src/FsmParse.h src/FsmParseList.c src/FsmParseList.h src/FsmMorphologicalAnalyzer.c src/FsmMorphologicalAnalyzer.h)
target_link_libraries(MorphologicalAnalysis ${CONAN_LIBS})
