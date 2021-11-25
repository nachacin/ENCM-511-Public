/* 
 * Course:    ENCM 511
 * Section:   L02 - B04
 * Group:     4
 * Author: Yahia Abrini, Nasih Nazeem, Nestor Chacin
 * Created on October 13, 2021, 5:29 PM
 */

#include "clocks.h"

/* PROMISES: This function is used to select between the different oscillators 
 * to improve the power efficiency of the program. By passing an integer as a
 * parameter, this will be used in a switch statement to select the appropriate
 * oscillator.
 * REQUIRES: A select integer to be passed and checked within a switch 
 * statement.
 */
void clock_switch(int selection) {
    char valid = 1;
    char COSCNOSC;
    
    // Switch statement to select the apprropriate oscillator depending on the
    // value of the arguement being passed into the function. 
    switch(selection) {
        case 8:
            COSCNOSC = 0x00;
            break;
        case 500:
            COSCNOSC = 0x66;
            break;
        case 32:
            COSCNOSC = 0x55;
            break;
        default:
            valid = 0;
            break;
    }
    if (valid) {
        SRbits.IPL = 7; // disable interrupts
        CLKDIVbits.RCDIV = 0; // turn clock division to zero
        __builtin_write_OSCCONH(COSCNOSC); // Changing of the oscillator
        __builtin_write_OSCCONL(0x01); // OSWEN bit is 1
        OSCCONbits.OSWEN = 1;
        while (OSCCONbits.OSWEN == 1) {}
        SRbits.IPL = 0; // enable interrupts
    }
}
