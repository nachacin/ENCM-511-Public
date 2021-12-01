/* 
 * Course:    ENCM 511
 * Section:   L02 - B04
 * Group:     4
 * Author: Nestor Chacin
 * Comments: This code was inspired by Geoffrey Hunter's work, who published a 
 * similar implementation as an example of a finite state machine. You can see 
 * his code in the following link:
 * https://github.com/gbmhunter/FunctionPointerStateMachineExample
 */

#include "flash_led.h"
#include "sub_state_machine.h"
#include "UART2.h"

/**
 * Maps a state to it's state transition function, which should be called
 * when the state transitions into this state.
 * WARNING: These have to be set in accordance to enumerated states in event_t 
 * type!
 */
static stateFunctionRow_t stateFunctionA[] = {
      // NAME         // FUNC
    { "ST_RESET",     &LED_reset },          // ST_RESET
    { "ST_LED_ON",    &LED_turn_on },        // ST_LED_ON
    { "ST_LED_OFF",   &LED_shut_off },       // ST_LED_OFF
};
/**
 * Contains all possible transitions from and to every possible state
 */
static stateTransMatrixRow_t stateTransMatrix[] = {
    // CURR STATE  // EVENT            // NEXT STATE
    { ST_RESET,    EV_RESET,           ST_RESET   },
    { ST_RESET,    EV_ONE_PRESSED,     ST_LED_ON  },
    { ST_RESET,    EV_TWO_PRESSED,     ST_LED_ON  },
    { ST_RESET,    EV_NONE_PRESSED,    ST_RESET   },
    { ST_LED_ON,   EV_RESET,           ST_RESET   },
    { ST_LED_ON,   EV_ONE_PRESSED,     ST_LED_OFF },
    { ST_LED_ON,   EV_TWO_PRESSED,     ST_LED_ON  },
    { ST_LED_ON,   EV_NONE_PRESSED,    ST_RESET   },
    { ST_LED_OFF,  EV_RESET,           ST_RESET   },
    { ST_LED_OFF,  EV_ONE_PRESSED,     ST_LED_ON  },
    { ST_LED_OFF,  EV_TWO_PRESSED,     ST_LED_ON  },
    { ST_LED_OFF,  EV_NONE_PRESSED,    ST_RESET   }
};
/**
 * PROMISES: Return the state machine to the ST_RESET state
 * REQUIRES: ST_RESET is mapped to a function that achieves the desired behavior
 * by the user
 * @param stateMachine
 */
void StateMachine_Reset(stateMachine_t * stateMachine) {
    Disp2String("Sub_State Machine has been reset");
    XmitUART2('\n', 1);
    XmitUART2('\r', 1);
    stateMachine->currState = ST_RESET;
    StateMachine_RunIteration(stateMachine, EV_RESET);
}

/**
 * Iterate through the state transition matrix, checking if there is both a 
 * match with the current state and the event. Once a match is found, the 
 * function corresponding to the next state is called and current state is
 * updated.
 * @param stateMachine
 * @param event
 */
void StateMachine_RunIteration(stateMachine_t *stateMachine, event_t event) {

    int i;
    for(i = 0; i < sizeof(stateTransMatrix)/sizeof(stateTransMatrix[0]); i++) {
        if(stateTransMatrix[i].currState == stateMachine->currState) {
            if(stateTransMatrix[i].event == event) {

                // Transition to the next state
                stateMachine->currState =  stateTransMatrix[i].nextState;

                // Call the function associated with transition
                (stateFunctionA[stateMachine->currState].func)();
                break;
            }
        }
    }
}

/**
 * PROMISES: Maps events to various combinations of inputs
 * REQUIRES: User has defined each case correctly prior to using this function.
 * @param PB1
 * @param PB2
 * @param PB3
 * @return 
 */
event_t get_curr_event(unsigned int PB1, unsigned int PB2, unsigned int PB3) {
    unsigned buttons = PB1 + (PB2<<1) + (PB3<<2);
    switch(buttons) {
        case 0x00:
            return EV_NONE_PRESSED;
        case 0x01:
        case 0x02:
        case 0x04:
            return EV_ONE_PRESSED;
        case 0x03:
        case 0x06:
        case 0x05:
        case 0x07:
            return EV_TWO_PRESSED;
        default:
            Disp2String("ERROR! get_curr_event did not reach a defined event");
            XmitUART2('\n', 1);
            XmitUART2('\r', 1);
            return EV_NONE_PRESSED;
    }
}