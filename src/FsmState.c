//
// Created by Olcay Taner YILDIZ on 14.10.2023.
//

#include <StringUtils.h>
#include <HashMap/HashMap.h>
#include <Memory/Memory.h>
#include "FsmState.h"

/**
 * First constructor of the Fsm_State class which takes 3 parameters String name, boolean startState,
 * and boolean endState as input and initializes the private variables of the class also leaves pos as null.
 *
 * @param name       String input.
 * @param start_state boolean input.
 * @param end_state   boolean input.
 */
Fsm_State_ptr create_fsm_state(const char *name, bool start_state, bool end_state) {
    Fsm_State_ptr result = malloc_(sizeof(Fsm_State), "create_fsm_state");
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
Fsm_State_ptr create_fsm_state2(const char *name, bool start_state, bool end_state, const char *pos) {
    Fsm_State_ptr result = malloc_(sizeof(Fsm_State), "create_fsm_state2");
    result->name = str_copy(result->name, name);
    result->start_state = start_state;
    result->end_state = end_state;
    result->pos = str_copy(result->pos, pos);
    return result;
}

/**
 * Frees memory allocated for a state.
 * @param state State to be deallocated.
 */
void free_fsm_state(Fsm_State_ptr state) {
    free_(state->pos);
    free_(state->name);
    free_(state);
}

/**
 * Hash function for the state.
 * @param state State to be hashed.
 * @param N N of the hash function
 * @return Hash value of a state.
 */
unsigned int hash_function_fsm_state(const Fsm_State *state, int N) {
    return hash_function_string(state->name, N);
}

/**
 * Comparator function of state object. Compares two state objects according to their names lexicographically.
 * @param first First state to compare.
 * @param second Second state to compare.
 * @return -1 if the name of the first state comes before the name of the second state, 0 if they are equal, 1
 * otherwise.
 */
int compare_fsm_state(const Fsm_State *first, const Fsm_State *second) {
    return compare_string(first->name, second->name);
}

/**
 * Empty constructor for State object.
 * @return Empty allocated State object.
 */
Fsm_State_ptr create_fsm_state3() {
    Fsm_State_ptr result = malloc_(sizeof(Fsm_State), "create_fsm_state3");
    result->name = NULL;
    result->pos = NULL;
    result->start_state = false;
    result->end_state = false;
    return result;
}
