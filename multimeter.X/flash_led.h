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
 * PROMISES: LATB8 will be set to value defined by RESET_STATE constant in 
 * flash_led.c
 */
void LED_reset();
/* REQUIRES: LATB8 must be set as a digital output 
 * PROMISES: LATB8 will be set to logic high and idle for a time totalling to
 * milliseconds assigned to constant DEL_1 in flash_led.c
 */
void LED_ON1();
/* REQUIRES: LATB8 must be set as a digital output 
 * PROMISES: LATB8 will be set to logic low and idle for a time totalling to
 * milliseconds assigned to constant DEL_1 in flash_led.c
 */
void LED_OFF1();
/* REQUIRES: LATB8 must be set as a digital output 
 * PROMISES: LATB8 will be set to logic high and idle for a time totalling to
 * milliseconds assigned to constant DEL_2 in flash_led.c
 */
void LED_ON2();
/* REQUIRES: LATB8 must be set as a digital output 
 * PROMISES: LATB8 will be set to logic low and idle for a time totalling to
 * milliseconds assigned to constant DEL_2 in flash_led.c
 */
void LED_OFF2();
/* REQUIRES: LATB8 must be set as a digital output 
 * PROMISES: LATB8 will be set to logic high and idle for a time totalling to
 * milliseconds assigned to constant DEL_3 in flash_led.c
 */
void LED_ON3();
/* REQUIRES: LATB8 must be set as a digital output 
 * PROMISES: LATB8 will be set to logic low and idle for a time totalling to
 * milliseconds assigned to constant DEL_3 in flash_led.c
 */
void LED_OFF3();

#endif	/* FLASH_LED_H */

