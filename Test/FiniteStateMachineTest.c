//
// Created by Olcay Taner YILDIZ on 22.10.2023.
//

#include <stdio.h>
#include "../src/FiniteStateMachine.h"
#include "../src/Transition.h"

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
        transition_count += transitions->size;
    }
    if (end_state_count != 37){
        printf("End fsm_state count %d does not match\n", end_state_count);
    }
    if (transition_count != 779){
        printf("Transition count %d does not match\n", transition_count);
    }
    free_finite_state_machine(fsm);
}