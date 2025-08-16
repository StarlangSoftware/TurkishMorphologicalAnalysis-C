//
// Created by Olcay Taner YILDIZ on 22.10.2023.
//

#include <stdio.h>
#include <Memory/Memory.h>
#include "../src/FiniteStateMachine.h"

int main(){
    Finite_state_machine_ptr fsm = create_finite_state_machine("turkish_finite_state_machine.xml");
    if (fsm->states->size != 141){
        printf("Fsm_State count does not match\n");
    }
    int end_state_count = 0;
    int transition_count = 0;
    for (int i = 0; i < fsm->states->size; i++){
        Fsm_State_ptr state = array_list_get(fsm->states, i);
        if (state->end_state){
            end_state_count++;
        }
        Array_list_ptr transitions = get_transitions(fsm, state);
        if (transitions->size == 0){
            free_array_list(transitions, NULL);
        } else {
            transition_count += transitions->size;
        }
    }
    if (end_state_count != 37){
        printf("End fsm_state count %d does not match\n", end_state_count);
    }
    if (transition_count != 783){
        printf("Transition count %d does not match\n", transition_count);
    }
    free_finite_state_machine(fsm);
}