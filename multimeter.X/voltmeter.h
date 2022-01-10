/* 
 * File: 
 * Course:    ENCM 511
 * Section:   L02 - B04
 * Group:     4
 * Author: Yahia Abrini, Nasih Nazeem, Nestor Chacin
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef ADC_H
#define	ADC_H

#include <xc.h> // include processor files - each processor file is guarded. 

typedef struct {
    uint16_t w_number;
    uint16_t remainder;
} mixed_number_t;

/**
 * PROMISES: Initializes the ADC for the appropriate pin (Pin 8)
 * REQUIRES: Nothing is required.
 */
void ADCinit(void);
/**
 * Warning: buff_dest must point to eight uint16_t stored contiguously in memory
 * @param buff_dest
 */
void do_ADC(uint16_t *buff_dest);
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

/**
 *
 */
void DispADC(void);
/**
 * Returns quotient of integer division with conventional rounding, that is, if
 * on the real number line the quotient resides on 0.5 or closer to the next 
 * highest integer, the quotient is rounded up; else if it resides less than 0.5
 * from the next integer, it is rounded down.
 */
#endif	/* ADC_H */
