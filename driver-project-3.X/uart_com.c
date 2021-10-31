#include "uart_com.h"
#include "UART2.h"
#include "stat_update.h"

void uart_com(){
    switch(state){
        case 0:
            break;
        case 1:
            Disp2String("PB1 is pushed");
            break;
    }
}
