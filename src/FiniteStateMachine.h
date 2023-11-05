//
// Created by Olcay Taner YILDIZ on 18.10.2023.
//

#ifndef MORPHOLOGICALANALYSIS_FINITESTATEMACHINE_H
#define MORPHOLOGICALANALYSIS_FINITESTATEMACHINE_H

#include <ArrayList.h>
#include <HashMap/HashMap.h>
#include "FsmState.h"

struct finite_state_machine {
    Array_list_ptr states;
    Hash_map_ptr transitions;
};

typedef struct finite_state_machine Finite_state_machine;

typedef Finite_state_machine *Finite_state_machine_ptr;

Finite_state_machine_ptr create_finite_state_machine2();

Finite_state_machine_ptr create_finite_state_machine(const char *file_name);

void free_finite_state_machine(Finite_state_machine_ptr finite_state_machine);

bool is_valid_transition(Finite_state_machine_ptr finite_state_machine, const char *transition);

Fsm_State_ptr get_state(Finite_state_machine_ptr finite_state_machine, const char *name);

void add_fsm_transition(Finite_state_machine_ptr finite_state_machine,
                        Fsm_State_ptr from_state,
                        Fsm_State_ptr to_state,
                        char *with,
                        char *with_name);

void add_fsm_transition2(Finite_state_machine_ptr finite_state_machine,
                         Fsm_State_ptr from_state,
                         Fsm_State_ptr to_state,
                         char *with,
                         char *with_name,
                         char *to_pos);

Array_list_ptr get_transitions(Finite_state_machine_ptr finite_state_machine, Fsm_State_ptr state);

void free_array_list_with_transitions(Array_list_ptr transition_list);

#endif //MORPHOLOGICALANALYSIS_FINITESTATEMACHINE_H
