#include "UART2.h"
#include "timers.h"
#include "stat_update.h"

int PB1_push; // initializing PB1_push; variable holds most recent state of button
int PB2_push; // initializing PB2_push; variable holds most recent state of button
int PB3_push; // initializing PB3_push; variable holds most recent state of button
int state;    // Holds the current state of the buttons
int update;   // Acts as a boolean to check whether there is a pending update
int persist;

/* PROMISES: Depending on the state of each individual pushbutton, this function
 *           updates the state integer after passing the appropriate if-statement.
 *           Returns a value of 1 or 0 depending on whether the program is 
 *           sleeping or not.
 * REQUIRES: No parameters required.
 */
int stat_update() {
    IEC1bits.CNIE = 0;                      // disabling interrupts to prevent ground bounce
    
    // If statement to update the state depending on the buttons actively pressed.
    if ((PB1_push + PB2_push + PB3_push) == 3){
        state = 7;
    } else if (PB1_push && PB2_push) {
        state = 4;
    } else if (PB1_push && PB3_push) {
        state = 6;
    } else if (PB2_push && PB3_push) {
        state = 5;
    } else if (PB1_push == 1) {
        state = 1;
    } else if (PB2_push == 1) {
        state = 2;
    } else if (PB3_push == 1) {
        state = 3;
    } else {
        state = 8;
        update = 0;
        persist = 0;
        IEC1bits.CNIE = 1;
        return 1;
    }
    update = 0;
    persist = 1;
    IEC1bits.CNIE = 1;              // re-enables interrupts
    return 0;
}