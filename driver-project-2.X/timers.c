/*
 * Course:    ENCM 511
 * Section:   L02 - B04
 * Group:     4
 * Author: Yahia Abrini, Nasih Nazeem, Nestor Chacin
 */

/*
 *  Delay Function
 *  Inputs: tmr_ticks which is an unsigned int to be used for PR2
 *          idle_on which is a flag used to trigger idle mode
 *  Return: nothing
 *
 */
#include "timers.h"

void delay(uint16_t tmr_ticks, uint8_t idle_on) {
    
    //T2CON config
    T2CONbits.TSIDL = 0; //operate in idle mode
    T2CONbits.T32 = 0; // operate timer 2 as 16 bit timer
    T2CONbits.TCS = 0; // use internal clock
    // T2CONbits.TGATE = 0;
    
    //T2CON prescaler select
    T2CONbits.TCKPS0 = 1;
    T2CONbits.TCKPS1 = 1;
    
    
    // Timer 2 interrupt config
    IPC1bits.T2IP = 3;  //7 is highest and 1 is lowest priority
    IEC0bits.T2IE = 1;  //enable timer interrupt
    IFS0bits.T2IF = 0;  // Clear timer 2 flag
    int eq_ticks = tmr_ticks * 15.627;
    PR2 = eq_ticks;    //PR2 stores the target to trigger T2 interrupt
    TMR2 = 0;           //zero TMR2 register to start
    T2CONbits.TON = 1;  //start timer 2
    
    if(idle_on == 1)
    {
        Idle();         //Enter idle state if 
    }
    T2CONbits.TON = 0;  //Stop timer
    TMR2 = 0;           //zero TMR2 register on exit
    return;
}