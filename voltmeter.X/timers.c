/*
 * Course:    ENCM 511
 * Section:   L02 - B04
 * Group:     4
 * Author: Yahia Abrini, Nasih Nazeem, Nestor Chacin
 */

#include "timers.h"
#include "UART2.h"

/*
 *  Delay Function
 *  Inputs: tmr_ticks which is an unsigned int to be used for PR2
 *          idle_on which is a flag used to trigger idle mode
 *  Return: nothing
 */
void delay_ms(uint16_t ms, uint8_t idle_on) {
    
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
    uint16_t COSC = OSCCONbits.COSC;
    /* numbers come from simplifying the following function: 
     * ((Fosc/256)/2)*(delay_ms/1000); 
     * this equation takes into account the 256 pre-scaler, the Fosc/2, and
     * the ms to s conversion */  
    switch(COSC) {
        case 0:
            equivalent_ticks = 16 * ms;
            break;
        case 5:
            equivalent_ticks = ms / 17;
            break;
        case 6:
            equivalent_ticks = ms;
            break;
        default:
            equivalent_ticks = ms;
            break;
    } 
    
    PR2 = equivalent_ticks;    //PR2 stores the target to trigger T2 interrupt
    TMR2 = 0;           //zero TMR2 register to start
    T2CONbits.TON = 1;  //start timer 2
    
    if(idle_on == 1) {
        Disp2String("Going Idle...");
        XmitUART2('\n', 1);
        XmitUART2('\r', 1);
        XmitUART2('.', 13);
        XmitUART2('\n', 1);
        XmitUART2('\r', 1);
        Idle();         //Enter idle state
    }
    T2CONbits.TON = 0;  //Stop timer
    TMR2 = 0;           //zero TMR2 register on exit
    return;
}
/**
 * PROMISES: Begins a delay counter in Timer 3 for the requested amount of 
 * seconds.
 * REQUIRES: System clock needs to be 8MHz. An appropriate definition for Timer
 * 3 interrupt.
 */
void delay_s(uint16_t seconds) {
    //T3CON config
    T3CONbits.TSIDL = 0; //operate in idle mode
    T2CONbits.T32 = 0; // operate timer 2 and timer 3 as 16 bit timer
    T3CONbits.TCS = 0; // use internal clock
    
    //T3CON prescaler select
    T3CONbits.TCKPS0 = 1;
    T3CONbits.TCKPS1 = 1;
    /* Set Timer3 Input Clock Prescale Select bits to 1:256 */
    
    
    // Timer 2 interrupt config
    IPC2bits.T3IP = 3;  //7 is highest and 1 is lowest priority
    IEC0bits.T3IE = 1;  //enable timer interrupt
    IFS0bits.T3IF = 0;  // Clear timer 3 flag
    uint16_t equivalent_ticks = 15625 * seconds;
    PR3 = equivalent_ticks;    //PR3 stores the target to trigger T2 interrupt
    TMR3 = 0;           //zero TMR3 register to start
    T3CONbits.TON = 1;  //start timer 3
    return;
}
/*
 * PROMISES: Returns a user defined delay associated with parameters. If no
 *           cases are associated with the parameter combination, returns 0.
 * REQUIRES: User to define cases prior to calling.
 */
unsigned int PB_delay(unsigned int PB1, unsigned int PB2, unsigned int PB3) {
    unsigned buttons = PB1 + (PB2<<1) + (PB3<<2);
    switch(buttons) {
        case 0x01:
            return 1000;
        case 0x02:
            return 2000;
        case 0x04:
            return 3000;
        default:
            return 0;
    }
}
