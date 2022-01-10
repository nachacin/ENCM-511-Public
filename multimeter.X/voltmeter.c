/*
 * Course:    ENCM 511
 * Section:   L02 - B04
 * Group:     4
 * Author: Yahia Abrini, Nasih Nazeem, Nestor Chacin
 */

#include "voltmeter.h"
#include "UART2.h"
/**
 * Returns quotient of integer division with conventional rounding, that is, if
 * on the real number line the quotient resides on 0.5 or closer to the next 
 * highest integer, the quotient is rounded up; else if it resides less than 0.5
 * from the next integer, it is rounded down.
 */
static uint16_t int_div_rnd(uint16_t num, uint16_t divisor) {
    if ((num < 0) ^ (divisor < 0)) {
        return ((num - divisor/2)/divisor);
    } else {
        return ((num + divisor/2)/divisor);
    }
}

/**
 * PROMISES: Initializes the ADC for the appropriate pin (Pin 8)
 * REQUIRES: Nothing is required.
 */
void ADCinit(void) {
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
    AD1CON3bits.ADCS = 0b00000000;   // ADCS conversion clock select bits 64 Tcy. Ignored if using internal AD RC Clock
    // TAD = (2 / Fosc) * ADCS)
    // Tsample = SAMC * TAD
    // Tconv = 12 * TAD
    
    // MUX settings
    AD1CHSbits.CH0NB = 0;       // Ch0 MUXB -ve Input is VR -
    AD1CHSbits.CH0SB = 0b0101;  // Positive input is AN5/RA3/pin8 for MUXA
    AD1CHSbits.CH0NA = 0;       // Ch0 -ve Input is VR-
    AD1CHSbits.CH0SA = 0b0101;  // Positive input is AN5/RA3/pin8 for MUXA
    
    // IO port selection
    TRISAbits.TRISA3 = 1;       // Set pin 8/RA3/AN5 as input
    AD1PCFG = 0xFFFF;           // Set all bits to digital
    AD1PCFGbits.PCFG5 = 0;      // Set only pin 8/RA3/AN5 as Analog input for ADC
    AD1CSSL = 0;                // Input scan disabled, 0x0000 is default case
    
    // INT bit setup
    IPC3bits.AD1IP = 5;
    AD1CON1bits.ADON = 1;       // Turn on ADC module
    return;
}
/**
 * Warning: buff_dest must point to eight uint16_t stored contiguously in memory
 * @param buff_dest
 */
void do_ADC(uint16_t *buff_dest) {    
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
void DispADC(void) {
    uint8_t ctr = 0;
    mixed_number_t ADC_value = {0 , 0};
    const uint16_t N = 125;
    const uint16_t buff_len = 8;
    const uint16_t sample_n = N * buff_len;     // Introducing a limit on the total samples that can be averaged successfully
    uint16_t buff_hold[buff_len];
    uint8_t i = 0;
    while (ctr != N){
        do_ADC(buff_hold);
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
    const uint16_t bar_graph_scale = 34; // resulting in approximately 1 step per 30 increments -> (1024 / 34)
    const uint16_t voltage_scale = 3;    // resulting in approximately mV:raw_ave of 3000:1000
    const uint16_t spacing = 32;
    uint16_t raw_ave = ADC_value.w_number + (ADC_value.remainder / sample_n);
    uint16_t bars = int_div_rnd(raw_ave, bar_graph_scale);
//    XmitUART2('|', bars);
//    XmitUART2('_', spacing - bars);
    Disp2String_newLine(1);
    Disp2Dec(raw_ave * voltage_scale);
    Disp2String_newLine("mV");
    return;
}

/**
 * This function allows a running average to be calculated by essentially 
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
//uint16_t average_array(uint16_t *array, uint16_t N) {
//    int x = 0; 
//    int y = 0;
//    int i;
//    for (i = 0; i < N; i++) { 
//        x += array[i] / N; 
//        int b = array[i] % N; 
//        if (y >= N - b) { 
//            x++; 
//            y -= N - b; 
//        } else { 
//            y += b; 
//        } 
//    }
//    return x + y/N;
//}