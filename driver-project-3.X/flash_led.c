#include "timers.h"
#include "stat_update.h"
static unsigned int ctr_delay1 = 500;             // approx 1 sec (1000 ms) delay
static unsigned int ctr_delay2 = 2000;             // approx 2 sec (2000 ms) delay
static unsigned int ctr_delay3 = 3000;             // approx 3 sec (3000 ms) delay
unsigned int flag_idle = 1;             // Flag to initiate idle() processor state    

/*PROMISES: This function checks the state of the device and then updates the
 * output pin (LED) and initiates a delay sequence for that output pin if 
 * necessary.
 *
 * REQUIRES: No parameters required.
 */
void flash_led() {
    switch(state) {
        case 1:     // PB1 is Pressed
            LATBbits.LATB8 = 1;             //turn on LED when button pressed 
            delay (ctr_delay1, flag_idle);  //initiate a delay (one shot)
            LATBbits.LATB8 = 0;             //turn off LED
            delay (ctr_delay1, flag_idle);  //initiate a delay (one shot)
            break;
        case 2:     // PB2 is Pressed
            LATBbits.LATB8 = 1;             //turn on LED when button pressed 
            delay (ctr_delay2, flag_idle);  //initiate a delay (one shot)
            LATBbits.LATB8 = 0;             //turn off LED
            delay (ctr_delay2, flag_idle);  //initiate a delay (one shot)
            break;
        case 3:     // PB3 is Pressed
            LATBbits.LATB8 = 1;             //turn on LED when button pressed 
            delay (ctr_delay3, flag_idle);  //initiate a delay (one shot)
            LATBbits.LATB8 = 0;             //turn off LED
            delay (ctr_delay3, flag_idle);  //initiate a delay (one shot)
            break;
        case 4:     // PB1 & PB2 are Pressed
        case 5:     // PB2 & PB3 are Pressed
        case 6:     // PB1 & PB3 are Pressed
        case 7:     // All PBs Pressed
            LATBbits.LATB8 = 1;             // turn on LED when button pressed 
            delay (ctr_delay1, flag_idle);  // initiate a delay (one shot)
            break;
        case 8:     // Nothing Pressed
        default:
            LATBbits.LATB8 = 0;             //turn off LED as a default state
            break;
    }
}
/* Requires: LATB8 must be set as a digital output 
 * Promises: LATB8 will be set to logic low voltage
 */
void LED_shut_off() {
    LATBbits.LATB8 = 0;
}