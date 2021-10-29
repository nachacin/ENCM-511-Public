#include "clocks.h"

void clock_switch(int selection) {
    char valid = 1;
    char COSCNOSC;
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
        __builtin_write_OSCCONH(COSCNOSC);
        __builtin_write_OSCCONL(0x01);
        OSCCONbits.OSWEN = 1;
        while (OSCCONbits.OSWEN == 1) {}
        SRbits.IPL = 0; // enable interrupts
    }
}
