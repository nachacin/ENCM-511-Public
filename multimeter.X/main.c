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
#include "voltmeter.h"
#include "frequency.h"
#include "Ohmmeter.h"
#include "Capmeter.h"

static void ev_react(top_event_t event) {
    Disp_transition_delim();
    switch(event){
        case EV_NONE_PRESSED:
            curr_TOP_state = TST_FREQUENCY;
            Disp2String_newLine("Changing top state to FREQUENCY");
            break;
        case EV_PB1_PRESSED:
            curr_TOP_state = TST_VOLTMETER;
            Disp2String_newLine("Changing top state to VOLTMETER");
            break;
        case EV_PB2_PRESSED:
            curr_TOP_state = TST_OHMMETER;
            Disp2String_newLine("Changing top state to OHMMETER");
            break;
        case EV_PB3_PRESSED:
            curr_TOP_state = TST_CAPACITANCE;
            Disp2String_newLine("Changing top state to CAPACITANCE");
            break;
        case EV_TWO_PRESSED:
            curr_TOP_state = TST_INT;
            Disp2String_newLine("Invalid input, can't press more than one button at a time!");
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
    
    // NOTE: Top states are declared as enumerations in interrupts.h
    while (1) {
        switch(curr_TOP_state) {
            case TST_FREQUENCY:
                get_frequency();
                IFS1bits.CNIF = 0;              // clear CN interrupt flags
                IEC1bits.CNIE = 1;              // enable CN interrupts
                Disp2String_newLine("Going to sleep...");
                Disp_new_line(1);   // Extra line to distinguish these transitions with more ease
                Sleep();
                break;
            case TST_INT:
            {
                IEC1bits.CNIE = 0;      // Disable Change notifications while determining changes. 
                                        // This may also help debounce.
                Disp2String_newLine("Button change detected...");
                ev_react(event_look_up());
                uart_PB_com();          // Reports which buttons are being pushed                                  // Enable change notifications.
                break;
            }
            case TST_VOLTMETER:
            {
                ADCinit();
                uint8_t i = 0;
                Disp2String("Voltage = ");
                while(i < 1) {
                    delay_ms(1000);
                    DispADC();
                    i++;
                    Idle();
                }
                curr_TOP_state = TST_INT;       // return to TST_INT after capacitance
                                                // measurement
                ev_react(update_PB_map());
                IFS1bits.CNIF = 0;              // clear CN interrupt flags
                IEC1bits.CNIE = 1;              // enable CN interrupts
                break;
            }
            case TST_OHMMETER:
                ADCinit_Resistance();           // Initializes ADC for pin 16
                Disp2String("Ohmeter Resistance = ");
                DispADC_Resistance();           // Gather values from ADC, Display them
                delay_ms(1000);                 // Delay for Idle()
                Idle();
                curr_TOP_state = TST_INT;       // return to TST_INT after capacitance
                                                // measurement
                ev_react(update_PB_map());
                IFS1bits.CNIF = 0;              // clear CN interrupt flags
                IEC1bits.CNIE = 1;              // enable CN interrupts
                break;
            case TST_CAPACITANCE:   
                initialize();               // initialize comparator & timer2
                start_T2(10);               // start the T2 interrupt sequence
                                            // (to count in intervals of 10 ms)
                LATBbits.LATB2 = 1;         // turn pin6 high to charge capacitor
                while(L_to_H == 0){
                    Idle();                 // stay in idle while a low-to-high
                                            // transition has not occurred
                }
                clock_switch(FRC);          // switch back to the FRC clock
                CM2CONbits.CON = 0;         // turn comparator off
                
                // Displaying Capacitance Output on the UART
                Disp2String("Capacitance = ");  
                //Disp2Dec(Cap_Time);
                int Cap_Val = Cap_Time * 0.05; // determine capacitance value from time
                //Disp2String("\n\r");
                Disp2Dec(Cap_Val);              // display capacitance value
                Disp2String_newLine(" uF");
                curr_TOP_state = TST_INT;       // return to TST_INT after capacitance
                                                // measurement
                delay_ms(3000);
                ev_react(update_PB_map());      // go to ev_react to update state
                IFS1bits.CNIF = 0;              // clear CN interrupt flags
                IEC1bits.CNIE = 1;              // enable CN interrupts
                break;
            default:
                Disp2String_newLine("Error: switch statement in main did not execute a defined case");
                Sleep();
                break;
        }
    }
    return (0);
}