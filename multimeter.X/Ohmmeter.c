/*
 * Course:    ENCM 511
 * Section:   L02 - B04
 * Group:     4
 * Author: Yahia Abrini, Nasih Nazeem, Nestor Chacin
 */

#include "Ohmmeter.h"
#include "UART2.h"

/**
 * PROMISES: Initializes the ADC for Ohmmeter and more specifically for
 * pin 16/AN11/RB13.
 * REQUIRES: Nothing is required.
 */
void ADCinit_Resistance(void) {
    AD1CON1bits.ADON = 0;           // Turn off ADC, turn on before sampling in do_ADC()
    AD1CON1bits.ADSIDL = 0;         // Continue ADC operation during idle mode
    AD1CON1bits.FORM = 0b00;        // 0b00: int, 01,: Signed Int, 10:Fractional, 11:Signed Fract
    AD1CON1bits.SSRC = 0b111;       // Start ADC conversion Time set by SAMC has passed
    AD1CON1bits.ASAM = 0;           // Next Sampling begins when SAMP bit is set
    AD1CON1bits.SAMP = 0;           // Will set this bit to 1 to start sampling in do_ADC()
    
    // Sampling time and ADC Clock select
    AD1CON2bits.VCFG = 0b000;       // Set Vref+ = AVDD and Vref- = AVSS
    AD1CON2bits.CSCNA = 0;          // Do not scan inputs; use channel selected by CH0SA bits
    AD1CON2bits.SMPI = 0b0111;      // Interrupt occurs every 8 samples
    AD1CON2bits.BUFM = 1;           // Buffer configured as a 2-8 word buffer
    AD1CON2bits.ALTS = 0;           // Always use MUXA input MUX settings
    AD1CON3bits.ADRC = 0;           // Clock derived from system clock
    AD1CON3bits.SAMC = 0b1000;      // Sample time (Kinda low - consider tweaking)
    AD1CON3bits.ADCS = 0b00000000;  // ADCS conversion clock select bits 64 Tcy. Ignored if using internal AD RC Clock
    // TAD = (2 / Fosc) * ADCS)
    // Tsample = SAMC * TAD
    // Tconv = 12 * TAD
    
    // MUX settings
    AD1CHSbits.CH0NB = 0;       // Ch0 MUXB -ve Input is VR -
    AD1CHSbits.CH0SB = 0b1011;  // Positive input is AN11/RB13/pin16 for MUXA
    AD1CHSbits.CH0NA = 0;       // Ch0 -ve Input is VR-
    AD1CHSbits.CH0SA = 0b1011;  // Positive input is AN11/RB13/pin16 for MUXA
    
    // IO port selection
    TRISBbits.TRISB13 = 1;       // Set pin 16/RB13/AN11 as input
    AD1PCFG = 0xFFFF;           // Set all bits to digital
    AD1PCFGbits.PCFG11 = 0;      // Set only pin 16/RB13/AN11 as Analog input for ADC
    AD1CSSL = 0;                // Input scan disabled, 0x0000 is default case
    
    // INT bit setup
    IPC3bits.AD1IP = 5;
    AD1CON1bits.ADON = 1;       // Turn on ADC module
    return;
}

/**
 * PROMISES: do_ADC_Resistance samples values read from RB13, and converts them.
 * REQUIRES: Array of unsigned integers that has a size of 8 bits.
 * Warning: buff_dest must point to eight uint16_t stored contiguously in memory
 * @param buff_dest
 */
void do_ADC_Resistance(uint16_t *buff_dest) {    
    IFS0bits.AD1IF = 0;         // Clear ADC interrupt flag
    AD1CON1bits.ASAM = 1;       // Auto start sampling for 31Tad
                                // then go to conversion
    while(!IFS0bits.AD1IF){}    // Converting...
    AD1CON1bits.ASAM = 0;       // Done. Stop Sampling
    if (AD1CON2bits.BUFS == 1) {
        buff_dest[0] = ADC1BUF0;
        buff_dest[1] = ADC1BUF1;
        buff_dest[2] = ADC1BUF2;
        buff_dest[3] = ADC1BUF3;
        buff_dest[4] = ADC1BUF4;
        buff_dest[5] = ADC1BUF5;
        buff_dest[6] = ADC1BUF6;
        buff_dest[7] = ADC1BUF7;
    } else {
        buff_dest[0] = ADC1BUF8;
        buff_dest[1] = ADC1BUF9;
        buff_dest[2] = ADC1BUFA;
        buff_dest[3] = ADC1BUFB;
        buff_dest[4] = ADC1BUFC;
        buff_dest[5] = ADC1BUFD;
        buff_dest[6] = ADC1BUFE;
        buff_dest[7] = ADC1BUFF;
    }
    return;
}

/**
 * PROMISES: Averages the values converted in do_ADC_Resistance and scales the 
 * raw average to be read in millivolts. This value is then used to calculate
 * the DUT resistor.
 * REQUIRES: Nothing is required.
 */
void DispADC_Resistance(void) {
    uint8_t ctr = 0;
    mixed_number_t ADC_value = {0 , 0};
    const uint16_t N = 125;
    const uint16_t buff_len = 8;
    const uint16_t sample_n = N * buff_len;     // Introducing a limit on the total samples that can be averaged successfully
    uint16_t buff_hold[buff_len];
    uint16_t res_value;
    uint8_t i = 0;
    while (ctr != N){
        do_ADC_Resistance(buff_hold);
        /**
         * This 4 loop allows a running average to be calculated by essentially 
         * using 2 16-bit integers to store whole and fractional magnitudes of 
         * each samples contribution to the average. This avoids precision loss 
         * due to repetitive rounding used in conventional averaging algorithms.
         * It was inspired by forum post written by Mark Gordon. For reference, 
         * see the following link:
         * https://www.quora.com/How-can-I-compute-the-average-of-a-large-array-of-integers-without-running-into-overflow
         */
        for (i = 0; i < buff_len; i++) {
            ADC_value.w_number += buff_hold[i] / sample_n; 
            int b = buff_hold[i] % sample_n; 
            if (ADC_value.remainder >= sample_n - b) { 
                ADC_value.w_number++; 
                ADC_value.remainder -= sample_n - b; 
            } else { 
                ADC_value.remainder += b; 
            } 
        } 
        ctr++;
    }
    
    const uint16_t voltage_scale = 3;    // resulting in approximately mV:raw_ave of 3000:1000
    uint16_t raw_ave = ADC_value.w_number + (ADC_value.remainder / sample_n);
    res_value = Measure_Resistance(raw_ave*voltage_scale);
    Disp2Dec(res_value);
    Disp2String_newLine("\n");
    return;
}

/**
 * PROMISES: Takes an average voltage value at RB13 and calculates the value
 * of the resistor under test. This is done by stating a constant current
 * limiting resistor of 1000 Ohms and a maximum voltage of 3025 Volts.
 * REQUIRES: An unsigned 16 bit integer value.
 * @param ave
 * @return unknown_r 
 */
uint16_t Measure_Resistance(uint16_t ave) {
    float known_r = 1000;
    float unknown_r;
    float max_v = 3025;
    
    unknown_r = (ave*known_r)/(max_v-ave);
    return(uint16_t) unknown_r;
}