/*
 * Course:    ENCM 511
 * Section:   L02 - B04
 * Group:     4
 * Author: Yahia Abrini, Nasih Nazeem, Nestor Chacin
 */

#include "gpio.h"

void io_init(void) {
    AD1PCFG = 0xFFFF;     // Turn all analog pins to digital
    TRISAbits.TRISA2 = 1; //Set A2 as an input (pushbutton input)
    TRISAbits.TRISA4 = 1; //Set A4 as an input (pushbutton input)
    TRISBbits.TRISB4 = 1; //Set B4 as an input (Pushbutton input)
    TRISBbits.TRISB8 = 0; //Set B8 as an output (LED output)
    TRISBbits.TRISB7 = 1; //Set B7 as an input (INT0 external interrupt)
    return;
}

void clock_ref_out(void) {
    TRISBbits.TRISB15 = 0;      // Set RB15 as output for REFO
    REFOCONbits.ROEN = 0;       // Ref oscillator is disabled to change control registers
    REFOCONbits.ROSSLP = 0;     // Ref oscillator is disabled in sleep
    REFOCONbits.ROSEL = 0;      // Output base clock showing clock signal
    REFOCONbits.RODIV = 0b1111;
    REFOCONbits.ROEN = 1;       // Enable Ref oscillator
    return;
}