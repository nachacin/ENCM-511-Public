/* 
  * Course:    ENCM 511
 * Section:   L02 - B04
 * Group:     4
 * Author: Yahia Abrini, Nasih Nazeem, Nestor Chacin
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef FLASH_LED_H
#define	FLASH_LED_H

#include <xc.h> // include processor files - each processor file is guarded.

/*PROMISES: This function checks the state of the device and then updates the
 * output pin (LED) and initiates a delay sequence for that output pin if 
 * necessary.
 *
 * REQUIRES: No parameters required.
 */
void flash_led();
/* Requires: LATB8 must be set as a digital output 
 * Promises: LATB8 will be set to logic low voltage
 */
void LED_shut_off();

#endif	/* FLASH_LED_H */