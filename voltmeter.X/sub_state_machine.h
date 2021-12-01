/* 
 * Course:    ENCM 511
 * Section:   L02 - B04
 * Group:     4
 * Author: Nestor Chacin
 * Comments: This code was inspired by Geoffrey Hunter, who published a similar 
 * implementation as an example of finite state machine. You can see his code in
 * the following link:
 * https://github.com/gbmhunter/FunctionPointerStateMachineExample
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef STATE_MACHINE_H
#define	STATE_MACHINE_H

#include <xc.h> // include processor files - each processor file is guarded.  

typedef enum {
    ST_RESET,
    ST_LED_ON,
    ST_LED_OFF
} state_t;
/* All the possible events that can occur for this state machine.
 * Unlike states_t, these do not need to be kept in a special order.
 */
typedef enum {
    EV_RESET,
    EV_ONE_PRESSED, 
    EV_TWO_PRESSED,    // In this case two or more
    EV_NONE_PRESSED
} event_t;
/**
 * A user defined type that defines a state -> event -> state sequence
 */
typedef struct {
    state_t currState;
    event_t event;
    state_t nextState;
} stateTransMatrixRow_t;

typedef struct {
    state_t currState;
} stateMachine_t;

typedef struct {
    const char * name;
    void (*func)(void);
} stateFunctionRow_t;

/**
 * PROMISES: Return the state machine to the ST_RESET state
 * REQUIRES: ST_RESET is mapped to a function that achieves the desired behavior
 * by the user
 * @param stateMachine
 */
void StateMachine_Reset(stateMachine_t * stateMachine);
/**
 * Iterate through the state transition matrix, checking if there is both a 
 * match with the current state and the event. Once a match is found, the 
 * function corresponding to the next state is called and current state is
 * updated.
 * @param stateMachine
 * @param event
 */
void StateMachine_RunIteration(stateMachine_t *stateMachine, event_t event);
/**
 * PROMISES: Maps events to various combinations of inputs
 * REQUIRES: User has defined each case correctly prior to using this function.
 * @param PB1
 * @param PB2
 * @param PB3
 * @return 
 */
event_t get_curr_event(unsigned int PB1, unsigned int PB2, unsigned int PB3);

#endif	/* STATE_MACHINE_H */

