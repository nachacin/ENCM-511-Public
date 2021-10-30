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

void delay(uint16_t delay_ms, uint8_t idle_on) {
    
    //T2CON config
    T2CONbits.TSIDL = 0; //operate in idle mode
    T2CONbits.T32 = 0; // operate timer 2 as 16 bit timer
    T2CONbits.TCS = 0; // use internal clock
    
    //T2CON prescaler select
    T2CONbits.TCKPS0 = 1;
    T2CONbits.TCKPS1 = 1;
    /* Set Timer2 Input Clock Prescale Select bits to 1:256 */
    
    
    // Timer 2 interrupt config
    IPC1bits.T2IP = 3;  //7 is highest and 1 is lowest priority
    IEC0bits.T2IE = 1;  //enable timer interrupt
    IFS0bits.T2IF = 0;  // Clear timer 2 flag
    
    
    int equivalent_ticks;
    // Pass the value that selects the current
    // oscillator.
    int selection = OSCCONbits.COSC; 
    
    
    // Switch statement to scale the amount of ticks to an appropriate number 
    // depending on what oscillator is picked.
    switch(selection) {
        case 111:
            equivalent_ticks = delay_ms*16;
            break;
        case 110:
            equivalent_ticks = delay_ms;
            break;
    }
    
    
    
    //int equivalent_ticks = delay_ms/0.064; 
    /* 0.064 comes from simplifying the following function: 
     * ((8000000/256)/2)*(delay_ms/1000); 
     * this function takes into account the 256 pre-scaler, the Fosc/2, and
     * the ms to s conversion */  
    
    PR2 = equivalent_ticks;    //PR2 stores the target to trigger T2 interrupt
    TMR2 = 0;           //zero TMR2 register to start
    T2CONbits.TON = 1;  //start timer 2
    
    if(idle_on == 1)
    {
        Idle();         //Enter idle state
    }
    T2CONbits.TON = 0;  //Stop timer
    TMR2 = 0;           //zero TMR2 register on exit
    return;
}