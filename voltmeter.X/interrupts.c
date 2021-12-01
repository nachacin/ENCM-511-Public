/*
 * Course:    ENCM 511
 * Section:   L02 - B04
 * Group:     4
 * Author: Yahia Abrini, Nasih Nazeem, Nestor Chacin
 */

#include "interrupts.h"
#include "UART2.h"

unsigned int PB1_push; // initializing PB1_push; variable holds most recent state of button
unsigned int PB2_push; // initializing PB2_push; variable holds most recent state of button
unsigned int PB3_push; // initializing PB3_push; variable holds most recent state of button

top_state_t curr_TOP_state; // Representing the state at the top of hierarchy

void interrupts_init(){
    /* Initialize change notification interrupt */
    CNEN2bits.CN30IE = 1; // enable change notification on RA2/CN30 pin
    CNEN1bits.CN0IE = 1; // enable change notification on RA4/CN0 pin
    CNEN1bits.CN1IE = 1; // enable change notification on RB4/CN1 pin
    /* set priority of change notification pins interrupts by writing to the CN 
     * pins in the interrupt priority control register, IPC4
     */
    IPC4bits.CNIP = 5;
    /* Clear change notification interrupt flag bit in the interrupt flag status
     * register, IFS1 
     */
    IFS1bits.CNIF = 0;
    /* Enable change notification interrupts in the interrupt enable control
     * register, IEC1
     */
    IEC1bits.CNIE = 1;
    IEC0bits.INT0IE = 1; // Enable INT0 External interrupt
    IPC0bits.INT0IP = 5; // Set priority for INT0 external interrupt
    INTCON2bits.INT0EP = 1; // INT0 interrupts on positive edge
    IFS0bits.INT0IF = 0; // Clear INT0 interrupt flag
}


void __attribute__((interrupt, no_auto_psv)) _T2Interrupt(void) {
     IFS0bits.T2IF = 0; //Clear timer 2 interrupt flag
     //Disp2String("\n\rT_2 ISR FIRED");
     return;
}
void __attribute__((interrupt, no_auto_psv)) _T3Interrupt(void) {
    IFS0bits.T3IF = 0; // Clear timer 3 interrupt flag
    T3CONbits.TON = 0;  //Stop timer
    TMR3 = 0;           //zero TMR3 register on exit
    //Disp2String("\n\rT_3 ISR FIRED");
}

void __attribute__((interrupt, no_auto_psv)) _CNInterrupt(void) {
    if (IFS1bits.CNIF == 1) {
        PB1_push = 0; // set PB1_push, mapped to RB4, to 0
        PB2_push = 0; // set PB2_push, mapped to RA4, to 0
        PB3_push = 0; // set PB3_push, mapped to RA2, to 0
        /* Capture new states in PORTA and PORTB explicitly as required by by CN
        module logic */
        int new_A = PORTA;
        int new_B = PORTB;
        /*Bit mask to determine most recent state of RB4 and update PB1_push*/
        if ((new_B & 0x10) == 0) {
            PB1_push = 1; // set PB1_push to 1
        }
        /*Bit mask to determine most recent state of RA4 and update PB2_push*/
        if ((new_A & 0x10) == 0) {
            PB2_push = 1; // set PB2_push to 1
        }
        /*Bit mask to determine most recent state of RA2 and update PB3_push*/
        if ((new_A & 0x04) == 0) {
            PB3_push = 1; // set PB3_push to 1
        }
        curr_TOP_state = TST_INT;
    }
    /* Clear input change notification interrupt flag status bit */
    IFS1bits.CNIF = 0;
    Nop();
    return;
}

/* PROMISES: Change top state to ON or Sleep as required
 * REQUIRES: Nothing
 */
unsigned int change_exec() {
    Disp2String("Button change detected...");
    XmitUART2('\n', 1);
    XmitUART2('\r', 1);
    if(PB1_push || PB2_push || PB3_push) {
        curr_TOP_state = TST_ON;
        XmitUART2('*', 60);
        XmitUART2('\n', 1);
        XmitUART2('\r', 1);
        Disp2String("Changing top state to TST_ON");
        XmitUART2('\n', 1);
        XmitUART2('\r', 1);
        return 1;
    } else {
        XmitUART2('*', 60);
        XmitUART2('\n', 1);
        XmitUART2('\r', 1);
        Disp2String("Changing top state to TST_SLEEP");
        XmitUART2('\n', 1);
        XmitUART2('\r', 1);
        curr_TOP_state = TST_SLEEP;
        return 0;
    }
}
/**
 * Interrupt service routine for external interrupt 0
 * REQUIRES: INT0 must be enabled again somewhere else in the program
 */
void __attribute__((interrupt, no_auto_psv)) _INT0Interrupt(void) {
    IEC0bits.INT0IE = 0; // Disable INT0 External Interrupt
    IEC1bits.CNIE = 0;
    curr_TOP_state = TST_PERSIST;
    XmitUART2('*', 60);
    XmitUART2('\n', 1);
    XmitUART2('\r', 1);
    Disp2String("Changing top state to TST_PERSIST");
    XmitUART2('\n', 1);
    XmitUART2('\r', 1);
    IFS0bits.INT0IF = 0; // Clear INT0 interrupt flag
    //Disp2String("\n\rINT0 FIRED");
    return;
}