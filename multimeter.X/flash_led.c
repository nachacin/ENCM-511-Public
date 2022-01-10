#include "flash_led.h"
#include "timers.h"

const static uint16_t RESET_STATE = 0;
const static uint16_t ON_STATE = 1;
const static uint16_t OFF_STATE = 0;
const static uint16_t DEL_1 = 1000;
const static uint16_t DEL_2 = 2000;
const static uint16_t DEL_3 = 3000;

/* REQUIRES: LATB8 must be set as a digital output 
 * PROMISES: LATB8 will be set to value defined by RESET_STATE constant in 
 * flash_led.c
 */
void LED_reset() {
    LATBbits.LATB8 = RESET_STATE;
}

/* REQUIRES: LATB8 must be set as a digital output 
 * PROMISES: LATB8 will be set to logic high and idle for a time totalling to
 * milliseconds assigned to constant DEL_1 in flash_led.c
 */
void LED_ON1(){
    LATBbits.LATB8 = ON_STATE;
    delay_ms(DEL_1);
}

/* REQUIRES: LATB8 must be set as a digital output 
 * PROMISES: LATB8 will be set to logic low and idle for a time totalling to
 * milliseconds assigned to constant DEL_1 in flash_led.c
 */
void LED_OFF1() {
    LATBbits.LATB8 = OFF_STATE;
    delay_ms(DEL_1);
}

/* REQUIRES: LATB8 must be set as a digital output 
 * PROMISES: LATB8 will be set to logic high and idle for a time totalling to
 * milliseconds assigned to constant DEL_2 in flash_led.c
 */
void LED_ON2(){
    LATBbits.LATB8 = ON_STATE;
    delay_ms(DEL_2);
}

/* REQUIRES: LATB8 must be set as a digital output 
 * PROMISES: LATB8 will be set to logic low and idle for a time totalling to
 * milliseconds assigned to constant DEL_2 in flash_led.c
 */
void LED_OFF2() {
    LATBbits.LATB8 = OFF_STATE;
    delay_ms(DEL_2);
}

/* REQUIRES: LATB8 must be set as a digital output 
 * PROMISES: LATB8 will be set to logic high and idle for a time totalling to
 * milliseconds assigned to constant DEL_3 in flash_led.c
 */
void LED_ON3(){
    LATBbits.LATB8 = ON_STATE;
    delay_ms(DEL_3);
}

/* REQUIRES: LATB8 must be set as a digital output 
 * PROMISES: LATB8 will be set to logic low and idle for a time totalling to
 * milliseconds assigned to constant DEL_3 in flash_led.c
 */
void LED_OFF3() {
    LATBbits.LATB8 = OFF_STATE;
    delay_ms(DEL_3);
}
