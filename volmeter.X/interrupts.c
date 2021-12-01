/*
 * Course:    ENCM 511
 * Section:   L02 - B04
 * Group:     4
 * Author: Yahia Abrini, Nasih Nazeem, Nestor Chacin
 */

#include "interrupts.h"
#include "UART2.h"

static const uint16_t PB1_MASK = 0x10; // For RB4
static const uint16_t PB2_MASK = 0x10; // For RA4
static const uint16_t PB3_MASK = 0x04; // For RA2

typedef enum {
    PB1_BIT = 0x01,
    PB2_BIT = 0x02,
    PB3_BIT = 0x04,
} button_bit_t ;

static uint16_t RA_field;
static uint16_t RB_field;
static uint16_t PB_mapped_field;
static uint16_t state_hold;

top_state_t curr_TOP_state; // Representing the state at the top of hierarchy

static void PB_map() {
    PB_mapped_field = 0;
    PB_mapped_field |= (RB_field & PB1_MASK)? 0x00 : PB1_BIT;
    PB_mapped_field |= (RA_field & PB2_MASK)? 0x00 : PB2_BIT;
    PB_mapped_field |= (RA_field & PB3_MASK)? 0x00 : PB3_BIT;
}

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
        RA_field = PORTA;
        RB_field = PORTB;
        curr_TOP_state = TST_INT;
    }
    /* Clear input change notification interrupt flag status bit */
    IFS1bits.CNIF = 0;
    Nop();
    return;
}

/**
 * Interrupt service routine for external interrupt 0
 * REQUIRES: INT0 must be enabled again somewhere else in the program
 */
void __attribute__((interrupt, no_auto_psv)) _INT0Interrupt(void) {
    IEC0bits.INT0IE = 0; // Disable INT0 External Interrupt
    IEC1bits.CNIE = 0;
    state_hold = curr_TOP_state;
    curr_TOP_state = TST_PERSIST;
    Disp_transition_delim();
    Disp2String_newLine("Changing top state to TST_PERSIST");
    IFS0bits.INT0IF = 0; // Clear INT0 interrupt flag
    //Disp2String("\n\rINT0 FIRED");
    return;
}

top_event_t event_look_up() {
    PB_map();
    switch(PB_mapped_field) {
        case 0x00:
            return EV_NONE_PRESSED;
        case PB1_BIT:
            return EV_PB1_PRESSED;
        case PB2_BIT:
            return EV_PB2_PRESSED;
        case PB3_BIT:
            return EV_PB3_PRESSED;
        case 0x03:
        case 0x05:
        case 0x06:
        case 0x07:
            return EV_TWO_PRESSED;
        default:
            Disp2String_newLine("ERROR! event_look_up() did not reach a defined event");
            return EV_NONE_PRESSED;
    }
}

void uart_PB_com(){
    switch(PB_mapped_field){
        case 0x00:
            Disp2String_newLine("Input: Nothing Pressed");
            break;
        case PB1_BIT:
            Disp2String_newLine("Input: PB1 is Pressed");
            break;
        case PB2_BIT:
            Disp2String_newLine("Input: PB2 is Pressed");
            break;
        case PB3_BIT:
            Disp2String_newLine("Input: PB3 is Pressed");
            break;
        case (PB1_BIT + PB2_BIT):
            Disp2String_newLine("Input: PB1 & PB2 are Pressed");
            break;
        case (PB1_BIT + PB3_BIT):
            Disp2String_newLine("Input: PB1 & PB3 are Pressed");
            break;
        case (PB2_BIT + PB3_BIT):
            Disp2String_newLine("Input: PB2 & PB3 are Pressed");
            break;
        case (PB1_BIT + PB2_BIT + PB3_BIT):
            Disp2String_newLine("Input: All PBs Pressed");
            break;
        default:
            Disp2String_newLine("ERROR! uart_PB_com received an undefined bit field");
            break;
    }
}

uint16_t get_PB_field() {
    return PB_mapped_field;
}

void continue_from_hold() {
    curr_TOP_state = state_hold;
}