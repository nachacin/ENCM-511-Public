/* 
 * File:   UART2.h
 * Author: rvyas
 *
 * Created on November 19, 2016, 9:22 PM
 */

#ifndef UART2_H
#define	UART2_H

void InitUART2(void);
void XmitUART2(char, unsigned int);

void __attribute__ ((interrupt, no_auto_psv)) _U2RXInterrupt(void);
void __attribute__ ((interrupt, no_auto_psv)) _U2TXInterrupt(void);

void Disp2Hex(unsigned int);
void Disp2Hex32(unsigned long int);
void Disp2String(char*);
void Disp2Dec(unsigned int);
/**
 * Wrapper function for XmitUART2() that transmits a new line and carriage 
 * return character encoded in ASCII to the terminal. The repeat parameter, 
 * defines how many times the process is repeated
 */
void Disp_new_line(unsigned int repeat);
/**
 * Wrapper function that displays a 'delimiter' on the terminal to mark the 
 * start of the program. The format consists of a line of '_' characters. number
 * of '_' used is defined by static constant termLineLength in the source file
 */
void Disp_boot_delim();
/**
 * Wrapper function that displays a 'delimiter' on the terminal to mark the 
 * transition between states at runtime. The format consists of a line of '*' 
 * characters. number of '*' used is defined by static constant termLineLength 
 * in the source file
 */
void Disp_transition_delim();
/**
 * Wrapper function that adds a new line and carriage return characters to 
 * behind strings sent to the terminal through UART
 */
void Disp2String_newLine(char *message);
#endif	/* UART2_H */
