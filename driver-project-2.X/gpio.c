#include "gpio.h"

void io_init(void) {
    AD1PCFG = 0xFFFF;     // Turn all analog pins to digital
    TRISAbits.TRISA2 = 1; //Set A2 as an input (pushbutton input)
    TRISAbits.TRISA4 = 1; //Set A4 as an input (pushbutton input)
    TRISBbits.TRISB4 = 1; //Set B4 as an input (Pushbutton input)
    TRISBbits.TRISB8 = 0; //Set B8 as an output (LED output)
    
    /* Initialize change notification interrupt */
    CNEN2bits.CN30IE = 1; // enable change notification on RA2/CN30 pin
    CNEN1bits.CN0IE = 1; // enable change notification on RA4/CN0 pin
    CNEN1bits.CN1IE = 1; // enable change notification on RB4/CN1 pin
    /* set priority of change notification pins interrupts by writing to the CN 
     * pins in the interrupt priority control register, IPC4
     */
    IPC4bits.CNIP = 5;
    /* Clear change notification interrupt flag bit in the interrupt flag status
     * register, IFS1 
     */
    IFS1bits.CNIF = 0;
    /* Enable change notification interrupts in the interrupt enable control
     * register, IEC1
     */
    IEC1bits.CNIE = 1;
    return;
}
