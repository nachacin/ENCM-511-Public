/* 
 * File: 
 * Course:    ENCM 511
 * Section:   L02 - B04
 * Group:     4
 * Author: Yahia Abrini, Nasih Nazeem, Nestor Chacin
 */

typedef enum{FRC = 0x00, LPFRC = 0x06, LPRC = 0x05} sys_clock_freq;

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef CLOCKS_H
#define	CLOCKS_H

#include <xc.h> // include processor files - each processor file is guarded.

/* PROMISES: This function is used to select between the different oscillators 
 * to improve the power efficiency of the program. By passing an enum as a
 * parameter, this will be used in a switch statement to select the appropriate
 * oscillator.
 * 
 * REQUIRES: An enum in sys_clock_freq to be passed and checked within a switch 
 * statement.
 */
void clock_switch(sys_clock_freq selection);
#endif

