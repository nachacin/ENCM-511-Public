/* 
 * File:   dp-2-main.c
 * Course:    ENCM 511
 * Section:   L02 - B04
 * Group:     4
 * Author: Nestor Chacin
 * Created on October 13, 2021, 5:29 PM
 */

// FBS
#pragma config BWRP = OFF               // Table Write Protect Boot (Boot segment may be written)
#pragma config BSS = OFF                // Boot segment Protect (No boot program Flash segment)

// FGS
#pragma config GWRP = OFF               // General Segment Code Flash Write Protection bit (General segment may be written)
#pragma config GCP = OFF                // General Segment Code Flash Code Protection bit (No protection)

// FOSCSEL
#pragma config FNOSC = FRCDIV           // Oscillator Select (8 MHz FRC oscillator with divide-by-N (FRCDIV))
#pragma config IESO = ON                // Internal External Switch Over bit (Internal External Switchover mode enabled (Two-Speed Start-up enabled))

// FOSC
#pragma config POSCMOD = NONE           // Primary Oscillator Configuration bits (Primary oscillator disabled)
#pragma config OSCIOFNC = OFF           // CLKO Enable Configuration bit (CLKO output signal is active on the OSCO pin)
#pragma config POSCFREQ = HS            // Primary Oscillator Frequency Range Configuration bits (Primary oscillator/external clock input frequency greater than 8 MHz)
#pragma config SOSCSEL = SOSCHP         // SOSC Power Selection Configuration bits (Secondary oscillator configured for high-power operation)
#pragma config FCKSM = CSDCMD           // Clock Switching and Monitor Selection (Both Clock Switching and Fail-safe Clock Monitor are disabled)

// FWDT
#pragma config WDTPS = PS32768          // Watchdog Timer Postscale Select bits (1:32,768)
#pragma config FWPSA = PR128            // WDT Prescaler (WDT prescaler ratio of 1:128)
#pragma config WINDIS = OFF             // Windowed Watchdog Timer Disable bit (Standard WDT selected; windowed WDT disabled)
#pragma config FWDTEN = ON              // Watchdog Timer Enable bit (WDT enabled)

// FPOR
#pragma config BOREN = BOR3             // Brown-out Reset Enable bits (Brown-out Reset enabled in hardware; SBOREN bit disabled)
#pragma config PWRTEN = ON              // Power-up Timer Enable bit (PWRT enabled)
#pragma config I2C1SEL = PRI            // Alternate I2C1 Pin Mapping bit (Default location for SCL1/SDA1 pins)
#pragma config BORV = V18               // Brown-out Reset Voltage bits (Brown-out Reset set to lowest voltage (1.8V))
#pragma config MCLRE = ON               // MCLR Pin Enable bit (MCLR pin enabled; RA5 input pin disabled)

// FICD
#pragma config ICS = PGx2               // ICD Pin Placement Select bits (PGC2/PGD2 are used for programming and debugging the device)

// FDS
#pragma config DSWDTPS = DSWDTPSF       // Deep Sleep Watchdog Timer Postscale Select bits (1:2,147,483,648 (25.7 Days))
#pragma config DSWDTOSC = LPRC          // DSWDT Reference Clock Select bit (DSWDT uses LPRC as reference clock)
#pragma config RTCOSC = SOSC            // RTCC Reference Clock Select bit (RTCC uses SOSC as reference clock)
#pragma config DSBOREN = ON             // Deep Sleep Zero-Power BOR Enable bit (Deep Sleep BOR enabled in Deep Sleep)
#pragma config DSWDTEN = ON             // Deep Sleep Watchdog Timer Enable bit (DSWDT enabled)

//Include the compiler structures
#include "xc.h"
#include <p24F16KA101.h>

int PB1_push = 0;
int PB2_push = 0;
int PB3_push = 0;
int CN0flag = 0; // Will represent a recent change of the RB4/CN1 pin
int CN1flag = 0; // Will represent a recent change of the RB4/CN1 pin
int CN30flag = 0; // Will represent a recent change of the RA2/CN30 pin

//Function declarations
void delay(uint16_t, uint8_t);
void gpio_init(void);
void __attribute__((interrupt, no_auto_psv)) _T2Interrupt(void);
void __attribute__((interrupt, no_auto_psv)) _CNInterrupt(void);


/*
 * Main code function - will be a while 1 structure that will execute forever
 */
int main(void) {
    CLKDIV = 0; //change default timing from 2:1 to 1:1
    
    unsigned int ctr_delay1 = 1;             // Initiate new variable to be used for PR2
    unsigned int ctr_delay2 = 1;            // Initiate alternate variable for PR2
    unsigned int ctr_delay3 = 1;            // Initiate alternate variable for PR2
    unsigned int flag_idle = 1;             // Flag to initiate idle() processor state
    
    ctr_delay1 = 10000;                     // approx 1 sec delay
    ctr_delay2 = ctr_delay1*2;              // approx 2 sec delay
    ctr_delay3 = ctr_delay1*3;              // approx 3 sec delay
    gpio_init();                            // Setup GPIO
    
    while (1) {
        if ((PB1_push + PB2_push + PB3_push) >= 2) {
            IEC1bits.CNIE = 0;
            LATBbits.LATB8 = 1;             //turn on LED when button pressed 
            delay (ctr_delay1, flag_idle);   //initiate a delay (one shot)
            IEC1bits.CNIE = 1;
        } else if (PB1_push == 1) {
            IEC1bits.CNIE = 0;
            LATBbits.LATB8 = 1;             //turn on LED when button pressed 
            delay (ctr_delay1, flag_idle);   //initiate a delay (one shot)
            LATBbits.LATB8 = 0;
            delay (ctr_delay1, flag_idle);   //initiate a delay (one shot)
            IEC1bits.CNIE = 1;
        } else if (PB2_push == 1) {
            IEC1bits.CNIE = 0;
            LATBbits.LATB8 = 1;             //turn on LED when button pressed 
            delay (ctr_delay2, flag_idle);   //initiate a delay (one shot)
            LATBbits.LATB8 = 0;
            delay (ctr_delay2, flag_idle);   //initiate a delay (one shot)
            IEC1bits.CNIE = 1;
        } else if (PB3_push == 1) {
            IEC1bits.CNIE = 0;
            LATBbits.LATB8 = 1;             //turn on LED when button pressed 
            delay (ctr_delay3, flag_idle);   //initiate a delay (one shot)
            LATBbits.LATB8 = 0;
            delay (ctr_delay3, flag_idle);   //initiate a delay (one shot)
            IEC1bits.CNIE = 1;
        } else {
            LATBbits.LATB8 = 0;             //turn off LED as a default state
        }
    }
    return (0);
}

void gpio_init(void) {
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

/*
 *  Delay Function
 *  Inputs: tmr_ticks which is an unsigned int to be used for PR2
 *          idle_on which is a flag used to trigger idle mode
 *  Return: nothing
 *
 */
void delay(uint16_t tmr_ticks, uint8_t idle_on)
{
    
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
    
    PR2 = tmr_ticks;    //PR2 stores the target to trigger T2 interrupt
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

/*
 *  Timer 2 Interrupt Service Routine
 *  Inputs: nothing
 *  Return: nothing
 */
void __attribute__((interrupt, no_auto_psv)) _T2Interrupt(void) {
     IFS0bits.T2IF = 0; //Clear timer 2 interrupt flag
     return;
}

void __attribute__((interrupt, no_auto_psv)) _CNInterrupt(void) {
    if (IFS1bits.CNIF == 1) {
        PB1_push = 0;
        PB2_push = 0;
        PB3_push = 0;
        if (PORTBbits.RB4 == 0) {
            PB1_push = 1;
        }
        if (PORTAbits.RA4 == 0){
            PB2_push = 1;
        }
        if (PORTAbits.RA2 == 0) {
            PB3_push = 1;
        }
    }
    /* Clear input change notification interrupt flag status bit */
    IFS1bits.CNIF = 0;
    Nop();
    return;
}
