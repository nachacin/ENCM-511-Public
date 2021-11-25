/* 
 * Course:    ENCM 511
 * Section:   L02 - B04
 * Group:     4
 * Author: Yahia Abrini, Nasih Nazeem, Nestor Chacin
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef UART_COM_H
#define	UART_COM_H

#include <xc.h> // include processor files - each processor file is guarded.

/* PROMISES: Depending on the state of the program, this function will 
 * communicate with a receiver relaying information about what buttons are being
 * pressed.
 * REQUIRES: No parameters required.
 */
void uart_com();

#endif	/* UART_COM_H */

