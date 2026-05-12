#include "delay.h"

void delay_ms(Timer_t* timer, uint32_t ms){
    timer->reset(); // make sure that ticks is 0
    timer->start();
    
    while((ms - timer->get_ticks()) != 0){
        __WFI();
    }
    timer->stop();
}