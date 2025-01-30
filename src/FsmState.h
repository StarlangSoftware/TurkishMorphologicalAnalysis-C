//
// Created by Olcay Taner YILDIZ on 14.10.2023.
//

#ifndef MORPHOLOGICALANALYSIS_FSMSTATE_H
#define MORPHOLOGICALANALYSIS_FSMSTATE_H

#include <stdbool.h>

struct fsm_state{
    bool start_state;
    bool end_state;
    char* name;
    char* pos;
};

typedef struct fsm_state Fsm_State;

typedef Fsm_State *Fsm_State_ptr;

Fsm_State_ptr create_fsm_state(const char* name, bool start_state, bool end_state);

Fsm_State_ptr create_fsm_state2(const char* name, bool start_state, bool end_state, const char* pos);

Fsm_State_ptr create_fsm_state3();

void free_fsm_state(Fsm_State_ptr state);

unsigned int hash_function_fsm_state(const Fsm_State *state, int N);

int compare_fsm_state(const Fsm_State *first, const Fsm_State *second);

#endif //MORPHOLOGICALANALYSIS_FSMSTATE_H
