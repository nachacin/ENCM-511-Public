/* 
 * Course:    ENCM 511
 * Section:   L02 - B04
 * Group:     4
 * Author: Yahia Abrini, Nasih Nazeem, Nestor Chacin
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef XC_HEADER_TEMPLATE_H
#define	XC_HEADER_TEMPLATE_H

#include <xc.h> // include processor files - each processor file is guarded.

extern int PB1_push; // declaring PB1_push
extern int PB2_push; // declaring PBN1_push
extern int PB3_push; // declaring PB3_push
extern int state;
extern int update;
extern int persist;

/* PROMISES: Depending on the state of each individual pushbutton, this function
 *           updates the state integer after passing the appropriate if-statement.
 *           Returns a value of 1 or 0 depending on whether the program is 
 *           sleeping or not.
 * REQUIRES: No parameters required.
 */
int stat_update();

#endif	/* XC_HEADER_TEMPLATE_H */

