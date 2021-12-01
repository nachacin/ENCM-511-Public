/* 
 * File: 
 * Course:    ENCM 511
 * Section:   L02 - B04
 * Group:     4
 * Author: Yahia Abrini, Nasih Nazeem, Nestor Chacin
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef TIMERS_H
#define	TIMERS_H

#include <xc.h>

/*
 *  Delay Function
 *  Inputs: uint16_t tmr_ticks which is an unsigned int to be used for PR2
 *          uint8_t idle_on which is a flag used to trigger idle mode
 *  Return: nothing
 *
 */
void delay_ms(uint16_t ms);
/**
 * PROMISES: Begins a delay counter in Timer 3 for the requested amount of 
 * seconds.
 * REQUIRES: System clock needs to be 8MHz. An appropriate definition for Timer
 * 3 interrupt.
 */
void delay_s(uint16_t seconds);

#endif

