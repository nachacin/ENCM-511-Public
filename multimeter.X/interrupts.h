/* 
 * File: 
 * Course:    ENCM 511
 * Section:   L02 - B04
 * Group:     4
 * Author: Yahia Abrini, Nasih Nazeem, Nestor Chacin
 */

#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include <xc.h>

// Declare all possible states at top of hierarchy
typedef enum {
    TST_FREQUENCY,
    TST_INT,
    TST_VOLTMETER,
    TST_OHMMETER,
    TST_CAPACITANCE,
    TST_INVALID,       
} top_state_t;

typedef enum {
    EV_NONE_PRESSED,
    EV_PB1_PRESSED,
    EV_PB2_PRESSED,
    EV_PB3_PRESSED,
    EV_TWO_PRESSED,    // In this case two or more
} top_event_t;

// Make this variable available across files that include this header
extern top_state_t curr_TOP_state;

unsigned extern int PB1_push; // declaring PB1_push
unsigned extern int PB2_push; // declaring PBN1_push
unsigned extern int PB3_push; // declaring PB3_push

unsigned extern int Comp_Out; // Output of comparator
unsigned extern int L_to_H;   // Status of Low to High Transition
unsigned extern int Cap_Time; // Time taken to discharge

void interrupts_init(void);
void __attribute__((interrupt, no_auto_psv)) _T2Interrupt(void);
/**
 * Timer3 ISR will serve the frequency.c module as it increments a static 
 * variable in this file with each rollover of TMR3. PR3 must be set to 0XFFFF 
 * for this to make sense
 */
void __attribute__((interrupt, no_auto_psv)) _T3Interrupt(void);

/**
 * PROMISES: Change of Notification Service Routine. Updates PB States if CN 
 * Flag is at a logic high, as well as updates top state to interrupt state.
 * REQUIRES: Nothing is required.
 */
void __attribute__((interrupt, no_auto_psv)) _CNInterrupt(void);


/**
 * PROMISES: Updates PB States according to PORTA or PORTB value changes
 * REQUIRES: Nothing is required.
 * @return  PB_mapped_field
 */
uint16_t update_PB_map();

/**
 * PROMISES: Comparator Interrupt Service Routine. Waits for COUT to output a 
 * logic high to then update the status of the Low to High transition state.
 * REQUIRES: Nothing is required.
 */
void __attribute__((interrupt, no_auto_psv)) _CompInterrupt(void);

/**
 * PROMISES: Returns the current event occurring according to the PB State.
 * REQUIRES: Nothing is required.
 * @return Current Event
 */
top_event_t event_look_up();
void uart_PB_com();

/**
 * PROMISES: Returns PB Field.
 * REQUIRES: PB_mapped_field
 * @return PB_mapped_field
 */
uint16_t get_PB_field();


#endif