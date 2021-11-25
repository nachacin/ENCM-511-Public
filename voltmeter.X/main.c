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
#include "uart_PB_com.h"
#include "ADC.h"

//#define Nop() {__asm__ volatile ("nop");}
//#define ClrWdt() {__asm__ volatile ("clrwdt");}
//#define Sleep() {__asm__ volatile ("pwrsav #0");}
//#define Idle() {__asm__ volatile ("pwrsav #1");}
//#define dsen() {__asm__ volatile ("BSET DSCON, #15");}


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
    clock_switch(FRC);              // Ensure oscillator is initially set to 8 MHz (Select 8 MHz)
    unsigned int flag_idle = 1;     // Flag to initiate idle() during timers
    unsigned int sub_state_delay;
    XmitUART2('\n', 1);             // Send new line to serial terminal to start session
    XmitUART2('\r', 1);
    XmitUART2('_', 60);
    XmitUART2('\n', 1);             // Send new line to serial terminal to start session
    XmitUART2('\r', 1);
    Disp2String("Booting up...");
    XmitUART2('\n', 1);
    XmitUART2('\r', 1);
    stateMachine_t sub_state_machine;
    ADCinit();
    // NOTE: Top states are declared as enumerations in interrupts.h
    while (1) {
        switch(curr_TOP_state) {
            case TST_SLEEP:
                StateMachine_Reset(&sub_state_machine); // Clean up sub_state
                Disp2String("Going to sleep...");  // For power usage monitoring
                XmitUART2('\n', 1);
                XmitUART2('\r', 1);
                XmitUART2('\n', 1);
                XmitUART2('\r', 1);
                Sleep();
                break;
            case TST_INT:
            {
                IEC1bits.CNIE = 0;                                  // Disable Change notifications while determining changes. This may also help debounce.
                unsigned int ON = change_exec();                    // Updates operating state due to an interrupt causing entry to TST_INT
                uart_PB_com(PB1_push, PB2_push, PB3_push);          // Finishes UART message started above
                /**
                 * CONSIDER: Deleting the control feature below since PB_delay 
                 * is a very small function anyway may as well execute it every 
                 * time.
                 */
                if(ON) {
                    sub_state_delay = PB_delay(PB1_push, PB2_push, PB3_push);  // Determines corresponding delay for each combination of input
                }
                IEC1bits.CNIE = 1;                              // Enable change notifications.
                break;
            }
            case TST_ON:
            {
                event_t next = get_curr_event(PB1_push, PB2_push, PB3_push);    // Determine next event given current input
                StateMachine_RunIteration(&sub_state_machine, next);            // Transition to the next state
                delay_ms (sub_state_delay, flag_idle);                             // Start delay associated with new state
                break;
            }
            case TST_PERSIST:
            {
                uint8_t i = 0;
                Disp2String("Voltage (100mV/bar):  ");
                XmitUART2('\n', 1);
                XmitUART2('\r', 1);
                while(i < 10) {
                    delay_s(1);
                    DispADC();
                    i++;
                    //Disp2String("\n\rGoing to Idle in TST_Persist");
                    Idle();
                }
                IFS0bits.INT0IF = 0;
                IEC0bits.INT0IE = 1; // Re-enable INT0 interrupts
                IEC1bits.CNIE = 1;
                change_exec();
                break;
            }
            default:
                Disp2String("Error: switch statement in main did not execute a defined case");
                XmitUART2('\n', 1);
                XmitUART2('\r', 1);
                Sleep();
                break;
        }
    }
    return (0);
}