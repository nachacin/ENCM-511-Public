/* 
 * File:      encm-511-dp-1.c
 * Course:    ENCM 511
 * Section:   L02 - B04
 * Group:     4
 * Author:    Nestor Chacin
 *
 * Created on September 28, 2021, 4:16 PM
 */

// PIC24F16KA101 Configuration Bit Settings

// 'C' source line config statements

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

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include <p24F16KA101.h>

/*
 * Program entry point
 */
int main(void) {
    /*
     * Set input pins. Was not able to use RA3 as it is only available when the
     * the primary oscillator is disabled.
     */
    TRISAbits.TRISA0 = 1;
    TRISAbits.TRISA1 = 1;
    TRISAbits.TRISA2 = 1;
    TRISBbits.TRISB13 = 1;
    /*
     * Set output pins. Since bits 11, 10, 6, 5, and 3 in PORTB are not
     * implemented in 20-pin pic24F's, pins RB0 - RB2, RB4, RB7 - RB9, and RB12
     * were selected to be used as outputs.
     */
    TRISBbits.TRISB0 = 0;
    TRISBbits.TRISB1 = 0;
    TRISBbits.TRISB2 = 0;
    TRISBbits.TRISB4 = 0;
    TRISBbits.TRISB7 = 0;
    TRISBbits.TRISB8 = 0;
    TRISBbits.TRISB9 = 0;
    TRISBbits.TRISB12 = 0;
    
    /*
     * Initialize an unsigned 16-bit integer variable which will be used to
     * store the state of input pins at the beginning of each iteration in the 
     * while loop below
     */
    uint16_t selection;
    /*
     * Begin a loop where a output pins are continously updated according to the
     * value of 'selection'. Each if and else-if statement forms a
     * hard-coded input-output relationship. If 'selection' is equal to 1-8,
     * the corresponding digit of my ucID, 10137614, will be written to output 
     * pins in the form of light patterns on an LED array. For example, if
     * selection is equal to 4, 4 LED's will turn on. Also, bit masking
     * operations are made to account for discontinuous ordering of IO pins.
     * For example, the 5th digit in my ucID would be written such that PORTB = 
     * 0000 0011 1001 0111. This represents the number 7, written to as logic
     * highs for the first 7 output pins initialized above. 
     */
    while(1) {
        /*
         * Read input pins through and store them in a variable after bit
         * masking and shifting operations
         */
        selection = (PORTA & 0x0007) | ((PORTB & 0x2000) >> 10);
        /*
         * input: 0000, output: 0
         */
        if (selection == 0x0000) {
            LATB = 0X0000;
        }
        /*
         * input: 0001, output: 1
         */
        else if (selection == 0x0001) {
            LATB = 0X0001;
        }
        /*
         * input: 0002, output: 0
         */
        else if (selection == 0x0002) {
            LATB = 0x0000;
        }
        /*
         * input: 0003, output: 1
         */
        else if (selection == 0x0003) {
            LATB = 0x0001;
        }
        /*
         * input: 0004, output: 3
         */
        else if (selection == 0x0004) {
            LATB = 0x0007;
        }
        /*
         * input: 0005, output: 7
         */
        else if (selection == 0x0005) {
            LATB = 0x0397;
        }
        /*
         * input: 0006, output: 6
         */
        else if (selection == 0x0006) {
            LATB = 0x0197;
        }
        /*
         * input: 0007, output: 1
         */
        else if (selection == 0x0007) {
            LATB = 0x0001;
        }
        /*
         * input: 0008, output: 4
         */
        else if (selection == 0x0008) {
            LATB = 0x0017;
        }
        /*
         * input: 0009 - 000F, output: 0
         */
        else if ((selection <= 0x000F) || (selection >= 0x0009)) {
            LATB = 0x0000;
        }
    }
    
    return (0);
}

