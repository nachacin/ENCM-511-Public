/* 
 * File:   Ohmmeter.h
 * Author: Nasih Nazeem
 *
 * Created on December 10, 2021, 6:04 PM
 */

#ifndef OHMMETER_H
#define	OHMMETER_H

#include <xc.h>
#include "voltmeter.h"


/**
 * PROMISES: Initializes the ADC for Ohmmeter and more specifically for
 * pin 16/AN11/RB13.
 * REQUIRES: Nothing is required.
 */
void ADCinit_Resistance(void);

/**
 * PROMISES: do_ADC_Resistance samples values read from RB13, and converts them.
 * REQUIRES: Array of unsigned integers that has a size of 8 bits.
 * Warning: buff_dest must point to eight uint16_t stored contiguously in memory
 * @param buff_dest
 */
/**
 * This functions allows a running average to be calculated by essentially 
 * using 2 16-bit integers to store whole and fractional magnitudes of 
 * each array element's contribution to the average. This avoids precision 
 * loss due to repetitive rounding used in conventional averaging algorithms.
 * It was inspired by forum post written by Mark Gordon. For reference, see 
 * the following link:
 * https://www.quora.com/How-can-I-compute-the-average-of-a-large-array-of-integers-without-running-into-overflow
 * 
 * This function is written into DispADC() for performance, but it is 
 * included here so that the algorithm may be inspected or used in the
 * future. If not in use, it should be commented out.
 */
// uint16_t average_array(uint16_t *array, uint16_t N);
void do_ADC_Resistance(uint16_t *buff_dest);


/**
 * PROMISES: Averages the values converted in do_ADC_Resistance and scales the 
 * raw average to be read in millivolts. This value is then used to calculate
 * the DUT resistor.
 * REQUIRES: Nothing is required.
 */
void DispADC_Resistance(void);


/**
 * PROMISES: Takes an average voltage value at RB13 and calculates the value
 * of the resistor under test. This is done by stating a constant current
 * limiting resistor of 1000 Ohms and a maximum voltage of 3025 Volts.
 * REQUIRES: An unsigned 16 bit integer value.
 * @param ave
 * @return unknown_r 
 */
uint16_t Measure_Resistance(uint16_t ave);




#endif	/* OHMMETER_H */

