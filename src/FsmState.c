//
// Created by Olcay Taner YILDIZ on 14.10.2023.
//

#include <stdlib.h>
#include <StringUtils.h>
#include <HashMap/HashMap.h>
#include "FsmState.h"

/**
 * First constructor of the Fsm_State class which takes 3 parameters String name, boolean startState,
 * and boolean endState as input and initializes the private variables of the class also leaves pos as null.
 *
 * @param name       String input.
 * @param start_state boolean input.
 * @param end_state   boolean input.
 */
Fsm_State_ptr create_fsm_state(char *name, bool start_state, bool end_state) {
    Fsm_State_ptr result = malloc(sizeof(Fsm_State));
    result->name = str_copy(result->name, name);
    result->start_state = start_state;
    result->end_state = end_state;
    result->pos = NULL;
    return result;
}

/**
 * Second constructor of the Fsm_State class which takes 4 parameters as input; String name, boolean startState,
 * boolean endState, and String pos and initializes the private variables of the class.
 *
 * @param name       String input.
 * @param start_state boolean input.
 * @param end_state   boolean input.
 * @param pos        String input.
 */
Fsm_State_ptr create_fsm_state2(char *name, bool start_state, bool end_state, char *pos) {
    Fsm_State_ptr result = malloc(sizeof(Fsm_State));
    result->name = str_copy(result->name, name);
    result->start_state = start_state;
    result->end_state = end_state;
    result->pos = str_copy(result->pos, pos);
    return result;
}

void free_fsm_state(Fsm_State_ptr state) {
    free(state->pos);
    free(state->name);
    free(state);
}

unsigned int hash_function_fsm_state(const Fsm_State *state, int N) {
    return hash_function_string(state->name, N);
}

int compare_fsm_state(const Fsm_State *first, const Fsm_State *second) {
    return compare_string(first->name, second->name);
}

Fsm_State_ptr create_fsm_state3() {
    Fsm_State_ptr result = malloc(sizeof(Fsm_State));
    result->name = NULL;
    result->pos = NULL;
    result->start_state = false;
    result->end_state = false;
    return result;
}
