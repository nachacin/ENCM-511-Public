/*
 * Course:    ENCM 511
 * Section:   L02 - B04
 * Group:     4
 * Author: Yahia Abrini, Nasih Nazeem, Nestor Chacin
 */

#include "interrupts.h"
#include "UART2.h"
#include "timers.h"

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
static uint16_t rollover_ctr = 0;

top_state_t curr_TOP_state; // Representing the state at the top of hierarchy

unsigned int Comp_Out;
unsigned int L_to_H;
unsigned int Cap_Time;

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
}


void __attribute__((interrupt, no_auto_psv)) _T2Interrupt(void) {
    
    if(curr_TOP_state == TST_CAPACITANCE){
        if(L_to_H == 0){            // if a low-to-high transition hasn't occurred
            Cap_Time = Cap_Time + 1;        // increase Cap_Time to keep track of time
            start_T2(10);                   // initialize T2 again using start_T2
        } else {                    // if a low-to-high transition has occurred
            LATBbits.LATB2 = 0;     // turn pin6 off
            T2CONbits.TON = 0;  //Stop timer
            TMR2 = 0;           //zero TMR2 register on exit
        }
    
    } else {
        T2CONbits.TON = 0; // Stop timer0
        TMR2 = 0;           //  ZERO TMR3 register on exit
    }
    
    //Disp2String_newLine("Timer 2 ISR fired!");
    IFS0bits.T2IF = 0; //Clear timer 2 interrupt flag
    return;
}
/**
 * Timer3 ISR will serve the frequency.c module as it increments a static 
 * variable in this file with each rollover of TMR3. PR3 must be set to 0XFFFF 
 * for this to make sense
 */
void __attribute__((interrupt, no_auto_psv)) _T3Interrupt(void) {
    rollover_ctr += 1;      // Increment rollover counter
    //Disp2String("\n\rT_3 ISR FIRED");
}

/**
 * PROMISES: Change of Notification Service Routine. Updates PB States if CN 
 * Flag is at a logic high, as well as updates top state to interrupt state.
 * REQUIRES: Nothing is required.
 */
void __attribute__((interrupt, no_auto_psv)) _CNInterrupt(void) {
    if (IFS1bits.CNIF == 1) {
        RA_field = PORTA;
        RB_field = PORTB;
        curr_TOP_state = TST_INT;
        IEC1bits.CNIE = 0;
    }
    /* Clear input change notification interrupt flag status bit */
    IFS1bits.CNIF = 0;
    Nop();
    return;
}

/**
 * PROMISES: Updates PB States according to PORTA or PORTB value changes
 * REQUIRES: Nothing is required.
 * @return  PB_mapped_field
 */
uint16_t update_PB_map() {
    RA_field = PORTA;
    RB_field = PORTB;
    PB_map();
    return PB_mapped_field;
}

/**
 * PROMISES: Comparator Interrupt Service Routine. Waits for COUT to output a 
 * logic high to then update the status of the Low to High transition state.
 * REQUIRES: Nothing is required.
 */
void __attribute__((interrupt, no_auto_psv)) _CompInterrupt(void) {
    Comp_Out = CM2CONbits.COUT;     // store the output of the comparator in Comp_Out
    
    // give value to L_to_H depending on whether there's a low-to-high transition
    // and whether more than 50 ms have passed (to avoid the first COMP interrupts
    // that get triggered when the comparator is turned on)
    if(Comp_Out == 1 && Cap_Time > 50) {  
        L_to_H = 1;                 // low-to-high transition
    } else {
        L_to_H = 0;                 // not low-to-high transition
    }
    
	IFS1bits.CMIF = 0;		// clear IF flag
    CM2CONbits.CEVT = 0;    // clear CEVT
        
    return;
}

/**
 * PROMISES: Returns the current event occurring according to the PB State.
 * REQUIRES: Nothing is required.
 * @return Current Event
 */
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

/**
 * PROMISES: Returns PB Field.
 * REQUIRES: PB_mapped_field
 * @return PB_mapped_field
 */
uint16_t get_PB_field() {
    return PB_mapped_field;
}