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
    TST_SLEEP,
    TST_INT,
    TST_FREQ,
    TST_PERSIST,
    TST_ON
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

void interrupts_init(void);
void __attribute__((interrupt, no_auto_psv)) _T2Interrupt(void);
void __attribute__((interrupt, no_auto_psv)) _T3Interrupt(void);
void __attribute__((interrupt, no_auto_psv)) _CNInterrupt(void);
void __attribute__((interrupt, no_auto_psv)) _INT0Interrupt(void);

top_event_t event_look_up();
void uart_PB_com();
uint16_t get_PB_field();
void continue_from_hold();

#endif