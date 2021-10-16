/* 
 * File:      dp-2-main.c
 * Course:    ENCM 511
 * Section:   L02 - B04
 * Group:     4
 * Author: Nestor Chacin
 * Created on October 13, 2021, 5:29 PM
 */


//Include the compiler structures
#include "xc.h"
#include <p24F16KA101.h>
#include "config.h"
#include "gpio.h"
#include "Timers.h"
#include "interrupts.h"

void __attribute__((interrupt, no_auto_psv)) _T2Interrupt(void);
void __attribute__((interrupt, no_auto_psv)) _CNInterrupt(void);

int PB1_push;
int PB2_push;
int PB3_push;
int CN0flag; // Will represent a recent change of the RB4/CN1 pin
int CN1flag; // Will represent a recent change of the RB4/CN1 pin
int CN30flag; // Will represent a recent change of the RA2/CN30 pin

/*
 * Main code function - will be a while 1 structure that will execute forever
 */
int main(void) {
    CLKDIV = 0; //change default timing from 2:1 to 1:1
    
    PB1_push = 0;
    PB2_push = 0;
    PB3_push = 0;
    CN0flag = 0; // Will represent a recent change of the RB4/CN1 pin
    CN1flag = 0; // Will represent a recent change of the RB4/CN1 pin
    CN30flag = 0; // Will represent a recent change of the RA2/CN30 pin
    
    unsigned int ctr_delay1 = 1;             // Initiate new variable to be used for PR2
    unsigned int ctr_delay2 = 1;            // Initiate alternate variable for PR2
    unsigned int ctr_delay3 = 1;            // Initiate alternate variable for PR2
    unsigned int flag_idle = 1;             // Flag to initiate idle() processor state
    
    ctr_delay1 = 10000;                     // approx 1 sec delay
    ctr_delay2 = ctr_delay1*2;              // approx 2 sec delay
    ctr_delay3 = ctr_delay1*3;              // approx 3 sec delay
    io_init();                            // Setup GPIO
    
    while (1) {
        if ((PB1_push + PB2_push + PB3_push) >= 2) {
            IEC1bits.CNIE = 0;
            LATBbits.LATB8 = 1;             //turn on LED when button pressed 
            delay (ctr_delay1, flag_idle);   //initiate a delay (one shot)
            IEC1bits.CNIE = 1;
        } else if (PB1_push == 1) {
            IEC1bits.CNIE = 0;
            LATBbits.LATB8 = 1;             //turn on LED when button pressed 
            delay (ctr_delay1, flag_idle);   //initiate a delay (one shot)
            LATBbits.LATB8 = 0;
            delay (ctr_delay1, flag_idle);   //initiate a delay (one shot)
            IEC1bits.CNIE = 1;
        } else if (PB2_push == 1) {
            IEC1bits.CNIE = 0;
            LATBbits.LATB8 = 1;             //turn on LED when button pressed 
            delay (ctr_delay2, flag_idle);   //initiate a delay (one shot)
            LATBbits.LATB8 = 0;
            delay (ctr_delay2, flag_idle);   //initiate a delay (one shot)
            IEC1bits.CNIE = 1;
        } else if (PB3_push == 1) {
            IEC1bits.CNIE = 0;
            LATBbits.LATB8 = 1;             //turn on LED when button pressed 
            delay (ctr_delay3, flag_idle);   //initiate a delay (one shot)
            LATBbits.LATB8 = 0;
            delay (ctr_delay3, flag_idle);   //initiate a delay (one shot)
            IEC1bits.CNIE = 1;
        } else {
            LATBbits.LATB8 = 0;             //turn off LED as a default state
        }
    }
    return (0);
}

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
