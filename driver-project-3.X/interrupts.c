/*
 * Course:    ENCM 511
 * Section:   L02 - B04
 * Group:     4
 * Author: Yahia Abrini, Nasih Nazeem, Nestor Chacin
 */

#include "interrupts.h"

int PB1_push; // initializing PB1_push; variable holds most recent state of button
int PB2_push; // initializing PB2_push; variable holds most recent state of button
int PB3_push; // initializing PB3_push; variable holds most recent state of button
int CN0flag;  // Will represent a recent change of the RB4/CN1 pin
int CN1flag;  // Will represent a recent change of the RB4/CN1 pin
int CN30flag; // Will represent a recent change of the RA2/CN30 pin

void __attribute__((interrupt, no_auto_psv)) _T2Interrupt(void) {
     IFS0bits.T2IF = 0; //Clear timer 2 interrupt flag
     return;
}

void __attribute__((interrupt, no_auto_psv)) _CNInterrupt(void) {
    if (IFS1bits.CNIF == 1) {
        PB1_push = 0; // set PB1_push to 0
        PB2_push = 0; // set PB2_push to 0
        PB3_push = 0; // set PB3_push to 0
        /* Capture new states in PORTA and PORTB explicitly as required by by CN
        module logic */
        int new_A = PORTA;
        int new_B = PORTB;
        if ((new_B & 0x10) == 0) {
            PB1_push = 1; // set PB1_push to 1
        }
        if ((new_A & 0x10) == 0) {
            PB2_push = 1; // set PB2_push to 1
        }
        if ((new_A & 0x04) == 0) {
            PB3_push = 1; // set PB3_push to 1
        }
    }
    /* Clear input change notification interrupt flag status bit */
    IFS1bits.CNIF = 0;
    Nop();
    return;
}