#include "timers.h"
#include "stat_update.h"
static unsigned int ctr_delay1 = 1000;             // approx 1 sec (1000 ms) delay
static unsigned int ctr_delay2 = 2000;             // approx 2 sec (2000 ms) delay
static unsigned int ctr_delay3 = 3000;             // approx 3 sec (3000 ms) delay
unsigned int flag_idle = 1;             // Flag to initiate idle() processor state    


void flash_led() {
    switch(state) {
        case 1:     // PB1 is Pressed
            LATBbits.LATB8 = 1;             //turn on LED when button pressed 
            delay (ctr_delay1, flag_idle);  //initiate a delay (one shot)
            LATBbits.LATB8 = 0;             //turn off LED
            delay (ctr_delay1, flag_idle);  //initiate a delay (one shot)
            break;
        case 2:     // PB2 is Pressed
            LATBbits.LATB8 = 1;             //turn on LED when button pressed 
            delay (ctr_delay2, flag_idle);  //initiate a delay (one shot)
            LATBbits.LATB8 = 0;             //turn off LED
            delay (ctr_delay2, flag_idle);  //initiate a delay (one shot)
            break;
        case 3:     // PB3 is Pressed
            LATBbits.LATB8 = 1;             //turn on LED when button pressed 
            delay (ctr_delay3, flag_idle);  //initiate a delay (one shot)
            LATBbits.LATB8 = 0;             //turn off LED
            delay (ctr_delay3, flag_idle);  //initiate a delay (one shot)
            break;
        case 4:     // PB1 & PB2 are Pressed
        case 5:     // PB2 & PB3 are Pressed
        case 6:     // PB1 & PB3 are Pressed
        case 7:     // All PBs Pressed
            LATBbits.LATB8 = 1;             // turn on LED when button pressed 
            delay (ctr_delay1, flag_idle);  // initiate a delay (one shot)
            break;
        case 8:     // Nothing Pressed
        default:
            LATBbits.LATB8 = 0;             //turn off LED as a default state
            break;
    }
}