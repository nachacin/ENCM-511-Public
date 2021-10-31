#include "uart_com.h"
#include "UART2.h"
#include "stat_update.h"

void uart_com(){
    switch(state){
        case 1:     // PB1 is Pressed
            Disp2String("PB1 is Pressed");
            break;
        case 2:     // PB2 is Pressed
            Disp2String("PB2 is Pressed");
            break;
        case 3:     // PB3 is Pressed
            Disp2String("PB3 is Pressed");
            break;
        case 4:     // PB1 & PB2 are Pressed
            Disp2String("PB1 & PB2 are Pressed");
            break;
        case 5:     // PB2 & PB3 are Pressed
            Disp2String("PB2 & PB3 are Pressed");
            break;
        case 6:     // PB1 & PB3 are Pressed
            Disp2String("PB1 & PB3 are Pressed");
            break;
        case 7:     // All PBs Pressed
            Disp2String("All PBs Pressed");
            break;
        case 8:     // Nothing Pressed
        default:
            Disp2String("Nothing Pressed");
            break;
    }
}
