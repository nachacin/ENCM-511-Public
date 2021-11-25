#include "timers.h"

/* REQUIRES: LATB8 must be set as a digital output 
 * PROMISES: LATB8 will be set to logic high voltage
 */
void LED_turn_on(){
    LATBbits.LATB8 = 1;
}
/* Requires: LATB8 must be set as a digital output 
 * Promises: LATB8 will be set to logic low voltage
 */
void LED_shut_off() {
    LATBbits.LATB8 = 0;
}
/* REQUIRES: LATB8 must be set as a digital output 
 * PROMISES: LATB8 will be set to a user defined initial state
 */
void LED_reset() {
    LATBbits.LATB8 = 0;
}