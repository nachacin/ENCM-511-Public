#include <xc.h>
#include "interrupts.h"
#include "UART2.h"
#define CAPTURE_BUFF_LENGTH 4

static uint16_t captures[CAPTURE_BUFF_LENGTH];
static const unsigned long TIME_BASE = 250000;

void get_frequency() {
    
    //// Timer3 Configuration
    //T3CON config
    T3CONbits.TSIDL = 0;        // operate in idle mode
    T2CONbits.T32 = 0;          // operate timer 2 and timer 3 as 16 bit timer
    T3CONbits.TCS = 0;          // use internal clock
    
    //T3CON prescaler select
    T3CONbits.TCKPS = 0b11;     // Set Timer3 Input Clock Prescale Select bits to 1:256
    
    
    // Timer 2 interrupt config
    IPC2bits.T3IP = 3;          // 7 is highest and 1 is lowest priority
    IEC0bits.T3IE = 0;          // Disable timer 3 interrupt
    IFS0bits.T3IF = 0;          // Clear timer 3 flag
    PR3 = 0xFFFF;               // Set Timer3 to interrupt at longest length of time possible
    TMR3 = 0;                   // Zero TMR3 register to start
    
    // Input Capture Configuration
    IPC0bits.IC1IP = 6;         // Set input compare interrupt priority level higher than timers
    IFS0bits.IC1IF = 0;         // Clear the IC1 interrupt status flag
    IEC0bits.IC1IE = 1;         // Enable IC1 interrupts
    
    IC1CONbits.ICM = 0b000;      // Input capture module is turned off
    
    IC1CONbits.ICSIDL = 0;      // Input Capture module will continue to operate in CPU Idle mode
    IC1CONbits.ICTMR = 0;       // TMR3 contents are captured on capture event
    IC1CONbits.ICI = 0b11;      // Interrupt on every 4th capture event
    
    // Initialize Input Capture
    
    T3CONbits.TON = 1;          // Start timer 3
    
    IC1CONbits.ICM = 0b101;     // Capture mode is turned ON, Capturing every 16th edge
    Idle();
    
    IEC0bits.IC1IE = 0;         // Disable IC1 interrupts
    IFS0bits.IC1IF = 0;         // Reset interrupt flag for input capture module
    T3CONbits.TON = 0;          // Stop timer 3
    
    //Disp2String_newLine("WE GOT HERE");
    
    IC1CONbits.ICM = 0b000;     // Capture mode is turned OFF
    signed int deltas[CAPTURE_BUFF_LENGTH - 1];
    unsigned char i;
    while (i < CAPTURE_BUFF_LENGTH) {
        deltas[i] = (signed int) captures[i + 1] - (signed int) captures[i];
        i++;
    }
    unsigned long total_delta = 0;
    i = 0;
    while (i < (CAPTURE_BUFF_LENGTH - 1)) {
        total_delta += (unsigned long) deltas[i];
        i++;
    }
    signed int ave_ticks = __builtin_divud(total_delta, (signed int) (CAPTURE_BUFF_LENGTH - 1));
    Disp2Hex32(__builtin_divud(TIME_BASE, ave_ticks));
    Disp2String_newLine("Hz");
}

void __attribute__((interrupt, no_auto_psv)) _IC1Interrupt(void) {
    IFS0bits.IC1IF = 0;         // Reset interrupt flag for input capture module
    uint8_t i = 0;
    while(IC1CONbits.ICBNE){    // While input capture buffer is NOT empty 
        captures[i] = IC1BUF;   // Read and save a capture entry from input capture buffer
        i++;
    }
    IC1CONbits.ICM = 0b000;
}