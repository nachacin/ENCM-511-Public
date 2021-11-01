/* 
 * Course:    ENCM 511
 * Section:   L02 - B04
 * Group:     4
 * Author: Yahia Abrini, Nasih Nazeem, Nestor Chacin
 */

#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include "xc.h"

extern int CN0flag;  // Will represent a recent change of the RB4/CN1 pin
extern int CN1flag;  // Will represent a recent change of the RB4/CN1 pin
extern int CN30flag; // Will represent a recent change of the RA2/CN30 pin

void __attribute__((interrupt, no_auto_psv)) _T2Interrupt(void);
void __attribute__((interrupt, no_auto_psv)) _CNInterrupt(void);

#endif