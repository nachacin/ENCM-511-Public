/* 
 * Course:    ENCM 511
 * Section:   L02 - B04
 * Group:     4
 * Author: Yahia Abrini, Nasih Nazeem, Nestor Chacin
 * Created on October 13, 2021, 5:29 PM
 */

#include <stdio.h> // include standard IO library - needed for printf()

//Include the compiler structures
#include "xc.h"
#include "config.h"
#include "gpio.h"
#include "timers.h"
#include "interrupts.h"
#include "UART2.h"
#include "clocks.h"
#include "stat_update.h"
#include "flash_led.h"
#include "uart_com.h"

/*
 * Main code function - will be a while 1 structure that will execute forever
 */
int main(void) {
    CLKDIV = 0; // change default timing from 2:1 to 1:1; keep frequency at 8MHz
    
    io_init();                             // Setup GPIO
    clock_switch(8); // Ensure oscillator is initially set to 8 MHz (Select 8 MHz)
    update = 1;  // trigger an update for startup
    int stat;
    
    // IF structure checks different pushbutton conditions to execute requested conditions
    while (1) {
        if (update == 1) {
            stat = stat_update();   // Update system operating state
            uart_com();             // Send uart messages corresponding to state
            while (persist) {
                flash_led();        // Produce LED pattern corresponding to state until a CN interrupt event
            }
            LED_shut_off();         // Shut off LED as a default state to return to
        }
        if (stat == 1) {            // Stat will equal 1 if system state is 8
            Sleep();                // Sleep until CN interrupt
        }
    }
    return (0);
}