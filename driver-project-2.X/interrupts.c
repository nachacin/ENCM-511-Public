/*
 * Course:    ENCM 511
 * Section:   L02 - B04
 * Group:     4
 * Author: Yahia Abrini, Nasih Nazeem, Nestor Chacin
 */

#include "interrupts.h"

int PB1_push;
int PB2_push;
int PB3_push;
int CN0flag; // Will represent a recent change of the RB4/CN1 pin
int CN1flag; // Will represent a recent change of the RB4/CN1 pin
int CN30flag; // Will represent a recent change of the RA2/CN30 pin


void __attribute__((interrupt, no_auto_psv)) _T2Interrupt(void) {
     IFS0bits.T2IF = 0; //Clear timer 2 interrupt flag
     return;
}

void __attribute__((interrupt, no_auto_psv)) _CNInterrupt(void) {
    if (IFS1bits.CNIF == 1) {
        PB1_push = 0;
        PB2_push = 0;
        PB3_push = 0;
        if (PORTBbits.RB4 == 0) {
            PB1_push = 1;
        }
        if (PORTAbits.RA4 == 0) {
            PB2_push = 1;
        }
        if (PORTAbits.RA2 == 0) {
            PB3_push = 1;
        }
    }
    /* Clear input change notification interrupt flag status bit */
    IFS1bits.CNIF = 0;
    Nop();
    return;
}