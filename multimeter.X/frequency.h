/* 
 * File:   frequency.h
 * Author: nesto
 *
 * Created on December 8, 2021, 11:35 PM
 */

#ifndef FREQUENCY_H
#define	FREQUENCY_H
/**
 * PROMISES: Initializes Timer3 and Input Capture modules as well as gathers the
 * frequency at Pin 14.
 * REQURIES: Nothing is required.
 */
void get_frequency();

/**
 * PROMISES: Input Capture Interrupt Service Routine
 * REQUIRES: Nothing is required.
 */
void __attribute__((interrupt, no_auto_psv)) _IC1Interrupt(void);

#endif	/* FREQUENCY_H */

