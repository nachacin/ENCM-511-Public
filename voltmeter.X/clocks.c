#include "clocks.h"

/* PROMISES: This function is used to select between the different oscillators 
 * to improve the power efficiency of the program. By passing an integer as a
 * parameter, this will be used in a switch statement to select the appropriate
 * oscillator.
 * REQUIRES: A select integer to be passed and checked within a switch 
 * statement.
 */
void clock_switch(sys_clock_freq selection) {
    char valid = 1;
    // Define variable for New Oscillator Selection Bits
    char NOSC;
    
    // Switch statement to select the appropriate oscillator depending on the
    // value of the argument being passed into the function. 
    switch(selection) {
        case FRC:
            NOSC = 0x00;
            break;
        case LPFRC:
            NOSC = 0x06;
            break;
        case LPRC:
            NOSC = 0x05;
            break;
        default:
            valid = 0;
            break;
    }
    if (valid) {
        SRbits.IPL = 7; // disable interrupts
        CLKDIVbits.RCDIV = 0; // turn clock division to zero
        __builtin_write_OSCCONH(NOSC); // Changing of the oscillator
        __builtin_write_OSCCONL(0x01); // OSWEN bit is 1
        OSCCONbits.OSWEN = 1;
        while (OSCCONbits.OSWEN == 1) {}
        SRbits.IPL = 0; // enable interrupts
    }
}