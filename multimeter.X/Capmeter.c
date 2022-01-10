/*
 * Course:    ENCM 511
 * Section:   L02 - B04
 * Group:     4
 * Author: Yahia Abrini, Nasih Nazeem, Nestor Chacin
 */

#include "Capmeter.h"
#include "clocks.h"
#include "interrupts.h"

/**
 * PROMISES: Initializes Timer 2 and Comparator
 * REQUIRES: Nothing is required.
 */
void initialize(){ 
    clock_switch(LPFRC);    // Switch clock to a slower one for the capacitance 
                            // lines to avoid truncation
    AD1PCFG = 0xFFFF;       // Set all pins to digital mode
    /* -------- COMPARATOR CONFIGURATION -------- */
    TRISAbits.TRISA6 = 0;
    TRISBbits.TRISB1 = 1; 	// Make pin5/RB1/AN3 an input
    AD1PCFGbits.PCFG3 = 0; 	// make pin5/RB1/AN3 an analog input
    
    TRISBbits.TRISB2 = 0; 	// Make pin6/RB2/CN6 an Output
    
    IEC1bits.CMIE = 0;      // disable CM interrupts while setting up pin
    CM2CONbits.COE = 1;  	// enable output pin
    CM2CONbits.CPOL = 0; 	// Comparator output is not inverted
    CM2CONbits.EVPOL = 3;   // Trigger/event/interrupt is generated on any 
                            // change of the comparator output
    CM2CONbits.CREF = 0; 	// pin5/C2InA pin is +in
    CM2CONbits.CCH = 3; 	// -in pin is connected to Vbg/2
    IFS1bits.CMIF = 0; 		// clear IF flag
    CM2CONbits.CEVT = 0;    // clear CEVT
    CM2CONbits.CON = 1; 	// turn comparator on

    IEC1bits.CMIE = 1;      // enable CM interrupts

    /* -------- TIMER 2 CONFIGURATION -------- */
    //T2CON Configuration
    T2CONbits.TSIDL = 0;    // operate in idle mode
    T2CONbits.T32 = 0;      // operate timer 2 as 16 bit timer
    T2CONbits.TCS = 0;      // use internal clock
    
    //T2CON Pre-Scaler select
    T2CONbits.TCKPS0 = 1;
    T2CONbits.TCKPS1 = 1;
    /* Set Timer2 Input Clock Pre-scale Select bits to 1:256 */
    
    // Timer 2 Interrupt Configuration
    IPC1bits.T2IP = 6;  // Set priority of T2 - 7 highest, 1 lowest priority
    IEC0bits.T2IE = 1;  // Enable timer interrupt
    IFS0bits.T2IF = 0;  // Clear timer 2 flag
    
    /* -------- GLOBAL VARIABLES -------- */
    Cap_Time = 0;       // Set Cap_Time to Zero at the Start
    return;
}