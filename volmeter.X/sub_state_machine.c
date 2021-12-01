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
static stateFunctionRow_t stateFunction[] = {
      // NAME         // FUNC
    { "ST_RESET",      &LED_reset },          // ST_RESET
    { "ST_LED_ON1",    &LED_ON1   },        // ST_LED_ON
    { "ST_LED_OFF1",   &LED_OFF1  },       // ST_LED_OFF
    { "ST_LED_ON2",    &LED_ON2   },
    { "ST_LED_OFF2",   &LED_OFF2  },
    { "ST_LED_ON3",    &LED_ON3   },
    { "ST_LED_OFF3",   &LED_OFF3  },
};
/**
 * Contains all possible transitions from and to every possible state
 */
static stateTransMatrixRow_t stateTransMatrix[] = {
    // CURR STATE  // EVENT            // NEXT STATE
    { ST_RESET     ,LED_EV_RESET        ,ST_RESET    },
    { ST_RESET     ,LED_EV_NONE_PRESSED ,ST_RESET    },
    { ST_RESET     ,LED_EV_PB1_PRESSED  ,ST_LED_ON1  },
    { ST_RESET     ,LED_EV_PB2_PRESSED  ,ST_LED_ON2  },
    { ST_RESET     ,LED_EV_PB3_PRESSED  ,ST_LED_ON3  },
    { ST_RESET     ,LED_EV_TWO_PRESSED  ,ST_LED_ON3  },
    { ST_LED_ON1   ,LED_EV_RESET        ,ST_RESET    },
    { ST_LED_ON1   ,LED_EV_NONE_PRESSED ,ST_RESET    },
    { ST_LED_ON1   ,LED_EV_PB1_PRESSED  ,ST_LED_OFF1 },
    { ST_LED_ON1   ,LED_EV_PB2_PRESSED  ,ST_LED_OFF2 },
    { ST_LED_ON1   ,LED_EV_PB3_PRESSED  ,ST_LED_OFF3 },
    { ST_LED_ON1   ,LED_EV_TWO_PRESSED  ,ST_LED_ON3  },
    { ST_LED_OFF1  ,LED_EV_RESET        ,ST_RESET    },
    { ST_LED_OFF1  ,LED_EV_NONE_PRESSED ,ST_RESET    },
    { ST_LED_OFF1  ,LED_EV_PB1_PRESSED  ,ST_LED_ON1  },
    { ST_LED_OFF1  ,LED_EV_PB2_PRESSED  ,ST_LED_ON2  },
    { ST_LED_OFF1  ,LED_EV_PB3_PRESSED  ,ST_LED_ON3  },
    { ST_LED_OFF1  ,LED_EV_TWO_PRESSED  ,ST_LED_ON3  },
    { ST_LED_ON2   ,LED_EV_RESET        ,ST_RESET    },
    { ST_LED_ON2   ,LED_EV_NONE_PRESSED ,ST_RESET    },
    { ST_LED_ON2   ,LED_EV_PB1_PRESSED  ,ST_LED_OFF1 },
    { ST_LED_ON2   ,LED_EV_PB2_PRESSED  ,ST_LED_OFF2 },
    { ST_LED_ON2   ,LED_EV_PB3_PRESSED  ,ST_LED_OFF3 },
    { ST_LED_ON2   ,LED_EV_TWO_PRESSED  ,ST_LED_ON3  },
    { ST_LED_OFF2  ,LED_EV_RESET        ,ST_RESET    },
    { ST_LED_OFF2  ,LED_EV_NONE_PRESSED ,ST_RESET    },
    { ST_LED_OFF2  ,LED_EV_PB1_PRESSED  ,ST_LED_ON1  },
    { ST_LED_OFF2  ,LED_EV_PB2_PRESSED  ,ST_LED_ON2  },
    { ST_LED_OFF2  ,LED_EV_PB3_PRESSED  ,ST_LED_ON3  },
    { ST_LED_OFF2  ,LED_EV_TWO_PRESSED  ,ST_LED_ON3  },
    { ST_LED_ON3   ,LED_EV_RESET        ,ST_RESET    },
    { ST_LED_ON3   ,LED_EV_NONE_PRESSED ,ST_RESET    },
    { ST_LED_ON3   ,LED_EV_PB1_PRESSED  ,ST_LED_OFF1 },
    { ST_LED_ON3   ,LED_EV_PB2_PRESSED  ,ST_LED_OFF2 },
    { ST_LED_ON3   ,LED_EV_PB3_PRESSED  ,ST_LED_OFF3 },
    { ST_LED_ON3   ,LED_EV_TWO_PRESSED  ,ST_LED_ON3  },
    { ST_LED_OFF3  ,LED_EV_RESET        ,ST_RESET    },
    { ST_LED_OFF3  ,LED_EV_NONE_PRESSED ,ST_RESET    },
    { ST_LED_OFF3  ,LED_EV_PB1_PRESSED  ,ST_LED_ON1  },
    { ST_LED_OFF3  ,LED_EV_PB2_PRESSED  ,ST_LED_ON2  },
    { ST_LED_OFF3  ,LED_EV_PB3_PRESSED  ,ST_LED_ON3  },
    { ST_LED_OFF3  ,LED_EV_TWO_PRESSED  ,ST_LED_ON3  }
};
/**
 * PROMISES: Return the state machine to the ST_RESET state
 * REQUIRES: ST_RESET is mapped to a function that achieves the desired behavior
 * by the user
 * @param stateMachine
 */
void StateMachine_Reset(stateMachine_t * stateMachine) {
    Disp2String_newLine("Sub_State Machine has been reset");
    stateMachine->currState = ST_RESET;
    (stateFunction[stateMachine->currState].func)();
}

/**
 * Iterate through the state transition matrix, checking if there is both a 
 * match with the current state and the event. Once a match is found, the 
 * function corresponding to the next state is called and current state is
 * updated. "Input" parameter modifies idle time.
 * @param stateMachine
 * @param event
 */
void StateMachine_RunIteration(stateMachine_t *stateMachine, uint16_t input) {
    event_t event = interpret_input(input);
    int i;
    for(i = 0; i < sizeof(stateTransMatrix)/sizeof(stateTransMatrix[0]); i++) {
        if(stateTransMatrix[i].currState == stateMachine->currState) {
            if(stateTransMatrix[i].event == event) {

                // Transition to the next state
                stateMachine->currState =  stateTransMatrix[i].nextState;

                // Call the function associated with transition
                (stateFunction[stateMachine->currState].func)();
                break;
            }
        }
    }
}

/**
 * PROMISES: Maps bit field representing state of array of buttons, input, to a 
 * corresponding event of interest to this particular state machine.
 * REQUIRES: User has defined each case correctly prior to using this function.
 * @return 
 */
event_t interpret_input(uint16_t input_bit_field) {
    switch(input_bit_field) {
        case 0x00:
            return LED_EV_NONE_PRESSED;
        case 0x01:
            return LED_EV_PB1_PRESSED;
        case 0x02:
            return LED_EV_PB2_PRESSED;
        case 0x04:
            return LED_EV_PB3_PRESSED;
        case 0x03:
        case 0x05:
        case 0x06:
        case 0x07:
            return LED_EV_TWO_PRESSED;
        default:
            Disp2String_newLine("ERROR! interpret_input() did not reach a defined event");
            return LED_EV_NONE_PRESSED;
    }
}