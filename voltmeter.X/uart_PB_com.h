/*
 * Course:    ENCM 511
 * Section:   L02 - B04
 * Group:     4
 * Author: Yahia Abrini, Nasih Nazeem, Nestor Chacin
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef UART_PB_COM_H
#define	UART_PB_COM_H

#include <xc.h> // include processor files - each processor file is guarded.  
/**
 * PROMISES: Will send appropriate UART messages associated with PB1, PB2 and 
 * PB3 states.
 * REQUIRES: The Disp2String() function defined in UART2.c must complete the 
 * actual UART transmission.
 */
void uart_PB_com(unsigned int PB1, unsigned int PB2, unsigned int PB3);
#endif	/* UART_PB_COM_H */

