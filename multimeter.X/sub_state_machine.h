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
    ST_LED_ON1,
    ST_LED_OFF1,
    ST_LED_ON2,
    ST_LED_OFF2,
    ST_LED_ON3,
    ST_LED_OFF3,            
} state_t;
/* All the possible events that can occur for this state machine.
 * Unlike states_t, these do not need to be kept in a special order.
 */
typedef enum {
    LED_EV_RESET,
    LED_EV_PB1_PRESSED,
    LED_EV_PB2_PRESSED,
    LED_EV_PB3_PRESSED,
    LED_EV_TWO_PRESSED,    // In this case two or more
    LED_EV_NONE_PRESSED
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
 * updated. "Input parameter modifies idle time.
 * @param stateMachine
 * @param event
 */
void StateMachine_RunIteration(stateMachine_t *stateMachine, uint16_t input);
/**
 * PROMISES: Maps bit field representing state of array of buttons, input, to a 
 * corresponding event of interest to this particular state machine.
 * REQUIRES: User has defined each case correctly prior to using this function.
 * @return 
 */
event_t interpret_input(uint16_t input_bit_field);

#endif	/* STATE_MACHINE_H */

