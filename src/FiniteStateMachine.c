//
// Created by Olcay Taner YILDIZ on 18.10.2023.
//

#include <XmlDocument.h>
#include <string.h>
#include <Memory/Memory.h>
#include "FiniteStateMachine.h"
#include "FsmState.h"
#include "Transition.h"

/**
 * Frees memory allocated for a finite state machine. Frees states and transitions.
 * @param finite_state_machine Finite state machine to be deallocated.
 */
void free_finite_state_machine(Finite_state_machine_ptr finite_state_machine) {
    free_array_list(finite_state_machine->states, (void (*)(void *)) free_fsm_state);
    free_hash_map(finite_state_machine->transitions, (void (*)(void *)) free_array_list_with_transitions);
    free_(finite_state_machine);
}

/**
 * Frees memory allocated to the transitions as an array.
 * @param transition_list Transition list to be deallocated.
 */
void free_array_list_with_transitions(Array_list_ptr transition_list){
    free_array_list(transition_list, (void (*)(void *)) free_transition);
}

/**
 * Creates an empty finite state machine with no empty state and transition arrays.
 * @return A new allocated finite state machine.
 */
Finite_state_machine_ptr create_finite_state_machine2() {
    Finite_state_machine_ptr result = malloc_(sizeof(Finite_state_machine), "create_finite_state_machine2");
    result->states = create_array_list();
    result->transitions = create_hash_map((unsigned int (*)(const void *, int)) hash_function_fsm_state,
                                          (int (*)(const void *, const void *)) compare_fsm_state);
    return result;
}

/**
 * Constructor reads the finite fsm_state machine in the given input file. It has a NodeList which holds the states
 * of the nodes and there are 4 different type of nodes; stateNode, root Node, transitionNode and withNode.
 * Also there are two states; fsm_state that a node currently in and fsm_state that a node will be in.
 * <p>
 * DOMParser is used to parse the given file. Firstly it gets the document to parse, then gets its elements by the
 * tag names. For instance, it gets states by the tag name 'fsm_state' and puts them into an ArrayList called stateList.
 * Secondly, it traverses this stateList and gets each Node's attributes. There are three attributes; name, start,
 * and end which will be named as states. If a node is in a startState it is tagged as 'yes', otherwise 'no'.
 * Also, if a node is in a startState, additional attribute will be fetched; originalPos that represents its original
 * part of speech.
 * <p>
 * At the last step, by starting rootNode's first child, it gets all the transitionNodes and next states called toState,
 * then continue with the nextSiblings. Also, if there is no possible toState, it prints this case and the causative states.
 *
 * @param file_name the resource file to read the finite fsm_state machine. Only files in resources folder are supported.
 */
Finite_state_machine_ptr create_finite_state_machine(const char *file_name) {
    Fsm_State_ptr state;
    Finite_state_machine_ptr result = create_finite_state_machine2();
    Xml_document_ptr doc = create_xml_document(file_name);
    parse(doc);
    Xml_element_ptr state_list_node = doc->root;
    Xml_element_ptr state_node = state_list_node->first_child;
    while (state_node != NULL) {
        if (has_attributes(state_node)) {
            char *state_name = get_attribute_value(state_node, "name");
            char *start_state = get_attribute_value(state_node, "start");
            char *end_state = get_attribute_value(state_node, "end");
            if (strcmp(start_state, "yes") == 0) {
                char *original_pos = get_attribute_value(state_node, "originalpos");
                state = create_fsm_state2(state_name, true, strcmp(end_state, "yes") == 0, original_pos);
            } else {
                state = create_fsm_state(state_name, false, strcmp(end_state, "yes") == 0);
            }
            array_list_add(result->states, state);
        }
        state_node = state_node->next_sibling;
    }
    state_node = state_list_node->first_child;
    while (state_node != NULL) {
        if (has_attributes(state_node)) {
            char *state_name = get_attribute_value(state_node, "name");
            state = get_state(result, state_name);
            Xml_element_ptr transition_node = state_node->first_child;
            while (transition_node != NULL) {
                if (has_attributes(transition_node)) {
                    char *to_state_name = get_attribute_value(transition_node, "name");
                    Fsm_State_ptr to_state = get_state(result, to_state_name);
                    char *with_name = get_attribute_value(transition_node, "transitionname");
                    char *root_to_pos = get_attribute_value(transition_node, "topos");
                    Xml_element_ptr with_node = transition_node->first_child;
                    while (with_node != NULL) {
                        char *to_pos;
                        if (has_attributes(with_node)) {
                            with_name = get_attribute_value(with_node, "name");
                            to_pos = get_attribute_value(with_node, "topos");
                        } else {
                            to_pos = NULL;
                        }
                        if (to_pos == NULL) {
                            if (root_to_pos == NULL) {
                                add_fsm_transition(result, state, to_state, with_node->pcData, with_name);
                            } else {
                                add_fsm_transition2(result, state, to_state, with_node->pcData, with_name, root_to_pos);
                            }
                        } else {
                            add_fsm_transition2(result, state, to_state, with_node->pcData, with_name, to_pos);
                        }
                        with_node = with_node->next_sibling;
                    }
                }
                transition_node = transition_node->next_sibling;
            }
        }
        state_node = state_node->next_sibling;
    }
    free_document(doc);
    return result;
}

/**
 * The isValidTransition loops through states ArrayList and checks transitions between states. If the actual transition
 * equals to the given transition input, method returns true otherwise returns false.
 *
 * @param finite_state_machine Current finite state machine object
 * @param transition is used to compare with the actual transition of a fsm_state.
 * @return true when the actual transition equals to the transition input, false otherwise.
 */
bool is_valid_transition(Finite_state_machine_ptr finite_state_machine, const char *transition) {
    Array_list_ptr list = value_list(finite_state_machine->transitions);
    for (int i = 0; i < list->size; i++) {
        Array_list_ptr transitions = array_list_get(list, i);
        for (int j = 0; j < transitions->size; j++) {
            Transition_ptr transition1 = array_list_get(transitions, j);
            if (transition1->with != NULL && strcmp(transition1->with, transition) == 0) {
                return true;
            }
        }
    }
    free_array_list(list, NULL);
    return false;
}

/**
 * The getState method is used to loop through the states ArrayList and return the fsm_state whose name equal
 * to the given input name.
 *
 * @param finite_state_machine Current finite state machine object
 * @param name is used to compare with the fsm_state's actual name.
 * @return fsm_state if found any, null otherwise.
 */
Fsm_State_ptr get_state(Finite_state_machine_ptr finite_state_machine, const char *name) {
    for (int i = 0; i < finite_state_machine->states->size; i++) {
        Fsm_State_ptr state = array_list_get(finite_state_machine->states, i);
        if (strcmp(state->name, name) == 0) {
            return state;
        }
    }
    return NULL;
}

/**
 * The addTransition method creates a new Transition with given input parameters and adds the transition to
 * transitions ArrayList.
 *
 * @param finite_state_machine Current finite state machine object
 * @param from_state  Fsm_State type input indicating the previous fsm_state.
 * @param to_state  Fsm_State type input indicating the next fsm_state.
 * @param with     String input indicating with what the transition will be made.
 * @param with_name String input.
 */
void add_fsm_transition(Finite_state_machine_ptr finite_state_machine,
                        Fsm_State_ptr from_state,
                        Fsm_State_ptr to_state,
                        char *with,
                        char *with_name) {
    Array_list_ptr transition_list;
    Transition_ptr new_transition = create_transition(to_state, with, with_name);
    if (hash_map_contains(finite_state_machine->transitions, from_state)){
        transition_list = hash_map_get(finite_state_machine->transitions, from_state);
        array_list_add(transition_list, new_transition);
    } else {
        transition_list = create_array_list();
        array_list_add(transition_list, new_transition);
        hash_map_insert(finite_state_machine->transitions, from_state, transition_list);
    }
}

/**
 * Another addTransition method which takes additional argument; to_pos and. It creates a new Transition
 * with given input parameters and adds the transition to transitions ArrayList.
 *
 * @param finite_state_machine Current finite state machine object
 * @param from_state  Fsm_State type input indicating the previous fsm_state.
 * @param to_state  Fsm_State type input indicating the next fsm_state.
 * @param with     String input indicating with what the transition will be made.
 * @param with_name String input.
 * @param to_pos    String input.
 */
void add_fsm_transition2(Finite_state_machine_ptr finite_state_machine,
                         Fsm_State_ptr from_state,
                         Fsm_State_ptr to_state,
                         char *with,
                         char *with_name,
                         char *to_pos) {
    Array_list_ptr transition_list;
    Transition_ptr new_transition = create_transition2(to_state, with, with_name, to_pos);
    if (hash_map_contains(finite_state_machine->transitions, from_state)){
        transition_list = hash_map_get(finite_state_machine->transitions, from_state);
        array_list_add(transition_list, new_transition);
    } else {
        transition_list = create_array_list();
        array_list_add(transition_list, new_transition);
        hash_map_insert(finite_state_machine->transitions, from_state, transition_list);
    }
}

/**
 * The get_transitions method returns the transitions at the given state.
 *
 * @param finite_state_machine Current finite state machine object
 * @param state State input.
 * @return transitions at given state.
 */
Array_list_ptr get_transitions(Finite_state_machine_ptr finite_state_machine, Fsm_State_ptr state) {
    if (hash_map_contains(finite_state_machine->transitions, state)){
        return hash_map_get(finite_state_machine->transitions, state);
    } else {
        return create_array_list();
    }
}
