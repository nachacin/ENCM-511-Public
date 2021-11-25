/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef FLASH_LED_H
#define	FLASH_LED_H

#include <xc.h> // include processor files - each processor file is guarded.  
 
/* REQUIRES: LATB8 must be set as a digital output 
 * PROMISES: LATB8 will be set to logic high voltage
 */
void LED_turn_on();
/* REQUIRES: LATB8 must be set as a digital output 
 * PROMISES: LATB8 will be set to logic low voltage
 */
void LED_shut_off();
/* REQUIRES: LATB8 must be set as a digital output 
 * PROMISES: LATB8 will be set to a user defined initial state
 */
void LED_reset();

#endif	/* FLASH_LED_H */

