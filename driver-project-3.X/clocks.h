/* 
 * Course:    ENCM 511
 * Section:   L02 - B04
 * Group:     4
 * Author: Yahia Abrini, Nasih Nazeem, Nestor Chacin
 */


// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef CLOCKS_H
#define	CLOCKS_H

#include "xc.h" // include processor files - each processor file is guarded.

/* PROMISES: This function is used to select between the different oscillators 
 * to improve the power efficiency of the program. By passing an integer as a
 * parameter, this will be used in a switch statement to select the appropriate
 * oscillator.
 * 
 * REQUIRES: A select integer to be passed and checked within a switch 
 * statement.
 */
void clock_switch(int selection);

#endif

