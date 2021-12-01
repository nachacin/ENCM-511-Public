/* 
 * File:      dp-2-main.c
 * Course:    ENCM 511
 * Section:   L02 - B04
 * Group:     4
 * Author: Nestor Chacin, Nasih Nazeem, Yahia Abrini
 * Created on October 13, 2021, 5:29 PM
 */

//Include the compiler structures
#include <xc.h> 
#include "config.h"
#include "gpio.h"
#include "timers.h"
#include "interrupts.h"
#include "UART2.h"
#include "clocks.h"
#include "flash_led.h"
#include "sub_state_machine.h"
#include "ADC.h"

static void ev_react(top_event_t event) {
    Disp_transition_delim();
    switch(event){
        case EV_NONE_PRESSED:
            curr_TOP_state = TST_FREQ;
            Disp2String_newLine("Changing top state to TST_FREQ");
            break;
        case EV_PB1_PRESSED:
            curr_TOP_state = TST_ON;
            Disp2String_newLine("Changing top state to TST_ON");
            break;
        case EV_PB2_PRESSED:
            curr_TOP_state = TST_ON;
            Disp2String_newLine("Changing top state to TST_ON");
            break;
        case EV_PB3_PRESSED:
            curr_TOP_state = TST_ON;
            Disp2String_newLine("Changing top state to TST_ON");
            break;
        case EV_TWO_PRESSED:
            curr_TOP_state = TST_ON;
            Disp2String_newLine("Changing top state to TST_ON");
            break;
        default:
            Disp2String_newLine("ERROR! undefined event passed to ev_react() did not reach a defined event");
            curr_TOP_state = EV_NONE_PRESSED;
            break;
    }
}

/*
 * Main code function - will be a while 1 structure that will execute forever.
 * A hierarchical state machine is implemented throughout the program. The while
 * loop contains a switch case that controls the "top state" of the MCU (Sleep,
 * Interrupted, and ON). Each case in the switch statement also contains an 
 * appropriate routine to execute clean-up, preparation, or behaviour as 
 * required.
 * 
 * WARNING: CN and External Interrupts respond to button pushes that are assumed
 * to include pull-up/pull-down resistors.
 */
int main(void) {
    CLKDIV = 0;                     // change default timing from 2:1 to 1:1; keep frequency at 8MHz
    io_init();                      // Setup GPIO
    interrupts_init();
    Disp_boot_delim();              // prints out a default delimiting line to distinguish the start of the program
    Disp2String_newLine("Booting up...");   // Distinguish start of program with a UART message
    stateMachine_t sub_state_machine;
    ADCinit();
    // NOTE: Top states are declared as enumerations in interrupts.h
    while (1) {
        switch(curr_TOP_state) {
            case TST_SLEEP:
                StateMachine_Reset(&sub_state_machine); // Clean up sub_state
                Disp2String_newLine("Going to sleep...");
                Disp_new_line(1);   // Extra line to distinguish these transitions with more ease
                Sleep();
                break;
            case TST_FREQ:
                StateMachine_Reset(&sub_state_machine);
                curr_TOP_state = TST_SLEEP;
                Disp2String_newLine("Changing top state to TST_SLEEP");
                Nop();
                break;
            case TST_INT:
            {
                IEC1bits.CNIE = 0;                                  // Disable Change notifications while determining changes. This may also help debounce.
                Disp2String_newLine("Button change detected...");
                ev_react(event_look_up());
                uart_PB_com();                                      // Reports which buttons are being pushed
                IEC1bits.CNIE = 1;                                  // Enable change notifications.
                break;
            }
            case TST_ON:
            {
                StateMachine_RunIteration(&sub_state_machine, get_PB_field());            // Transition to the next state
                break;
            }
            case TST_PERSIST:
            {
                uint8_t i = 0;
                Disp2String_newLine("Voltage (100mV/bar):  ");
                while(i < 10) {
                    delay_s(1);
                    DispADC();
                    i++;
                    Idle();
                }
                IFS0bits.INT0IF = 0;
                IEC0bits.INT0IE = 1; // Re-enable INT0 interrupts
                IEC1bits.CNIE = 1;
                continue_from_hold();
                break;
            }
            default:
                Disp2String_newLine("Error: switch statement in main did not execute a defined case");
                Sleep();
                break;
        }
    }
    return (0);
}