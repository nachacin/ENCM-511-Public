/*
 * Course:    ENCM 511
 * Section:   L02 - B04
 * Group:     4
 * Author: Yahia Abrini, Nasih Nazeem, Nestor Chacin
 */

#include "uart_PB_com.h"
#include "UART2.h"

/**
 * PROMISES: Will send appropriate UART messages associated with PB1, PB2 and 
 * PB3 states.
 * REQUIRES: The Disp2String() function defined in UART2.c must complete the 
 * actual UART transmission.
 */
void uart_PB_com(unsigned int PB1, unsigned int PB2, unsigned int PB3) {
    if (PB1 && PB2 && PB3){
        Disp2String("Input: All PBs Pressed");
        XmitUART2('\n', 1);
        XmitUART2('\r', 1);
    } else if (PB1 && PB2) {
        Disp2String("Input: PB1 & PB2 are Pressed");
        XmitUART2('\n', 1);
        XmitUART2('\r', 1);
    } else if (PB1 && PB3) {
        Disp2String("Input: Input: PB1 & PB3 are Pressed");
        XmitUART2('\n', 1);
        XmitUART2('\r', 1);
    } else if (PB2 && PB3) {
        Disp2String("Input: PB2 & PB3 are Pressed");
        XmitUART2('\n', 1);
        XmitUART2('\r', 1);
    } else if (PB1 == 1) {
        Disp2String("Input: PB1 is Pressed");
        XmitUART2('\n', 1);
        XmitUART2('\r', 1);
    } else if (PB2 == 1) {
        Disp2String("Input: PB2 is Pressed");
        XmitUART2('\n', 1);
        XmitUART2('\r', 1);
    } else if (PB3 == 1) {
        Disp2String("Input: PB3 is Pressed");
        XmitUART2('\n', 1);
        XmitUART2('\r', 1);
    } else {
        Disp2String("Input: Nothing Pressed");
        XmitUART2('\n', 1);
        XmitUART2('\r', 1);
    }
    return;
}
