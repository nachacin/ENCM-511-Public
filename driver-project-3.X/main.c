/* 
 * File:      dp-2-main.c
 * Course:    ENCM 511
 * Section:   L02 - B04
 * Group:     4
 * Author: Nestor Chacin, Nasih Nazeem, Yahia Abrini
 * Created on October 13, 2021, 5:29 PM
 */

//Include the compiler structures
#include "xc.h"
#include "config.h"
#include "gpio.h"
#include "timers.h"
#include "interrupts.h"
#include "UART2.h"
#include "clocks.h"

/*
 * Main code function - will be a while 1 structure that will execute forever
 */
int main(void) {
    CLKDIV = 0; // change default timing from 2:1 to 1:1; keep frequency at 8MHz
    
    unsigned int ctr_delay1 = 1;            // Initiate new variable to be used for PR2
    unsigned int ctr_delay2 = 1;            // Initiate alternate variable for PR2
    unsigned int ctr_delay3 = 1;            // Initiate alternate variable for PR2
    unsigned int flag_idle = 1;             // Flag to initiate idle() processor state
    
    ctr_delay1 = 1000;                     // approx 1 sec (1000 ms) delay
    ctr_delay2 = ctr_delay1*2;             // approx 2 sec (2000 ms) delay
    ctr_delay3 = ctr_delay1*3;             // approx 3 sec (3000 ms) delay
    io_init();                             // Setup GPIO
    clock_switch(8);
   
    // IF structure checks different pushbutton conditions to execute requested conditions
    while (1) {
        if ((PB1_push + PB2_push + PB3_push) >= 2) {
            IEC1bits.CNIE = 0;              // disabling interrupts to prevent ground bounce
            LATBbits.LATB8 = 1;             // turn on LED when button pressed 
            delay (ctr_delay1, flag_idle);  // initiate a delay (one shot)
            IEC1bits.CNIE = 1;              // re-enables interrupts 
        } else if (PB1_push == 1) {
            IEC1bits.CNIE = 0;              // disabling interrupts to prevent ground bounce
            LATBbits.LATB8 = 1;             //turn on LED when button pressed 
            delay (ctr_delay1, flag_idle);  //initiate a delay (one shot)
            LATBbits.LATB8 = 0;             //turn off LED
            delay (ctr_delay1, flag_idle);  //initiate a delay (one shot)
            IEC1bits.CNIE = 1;              // re-enables interrupts
            XmitUART2('J', 1);
        } else if (PB2_push == 1) {
            IEC1bits.CNIE = 0;              // disabling interrupts to prevent ground bounce
            LATBbits.LATB8 = 1;             //turn on LED when button pressed 
            delay (ctr_delay2, flag_idle);  //initiate a delay (one shot)
            LATBbits.LATB8 = 0;             //turn off LED
            delay (ctr_delay2, flag_idle);  //initiate a delay (one shot)
            IEC1bits.CNIE = 1;              // re-enables interrupts 
        } else if (PB3_push == 1) {
            IEC1bits.CNIE = 0;              // disabling interrupts to prevent ground bounce
            LATBbits.LATB8 = 1;             //turn on LED when button pressed 
            delay (ctr_delay3, flag_idle);  //initiate a delay (one shot)
            LATBbits.LATB8 = 0;             //turn off LED
            delay (ctr_delay3, flag_idle);  //initiate a delay (one shot)
            IEC1bits.CNIE = 1;              // re-enables interrupts 
        } else {
            LATBbits.LATB8 = 0;             //turn off LED as a default state
            Sleep();
        }
    }
    return (0);
}