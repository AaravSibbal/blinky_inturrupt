#ifndef DELAY_H
#define DELAY_H

#include "../../def.h"
#include "../../peripherals/timers/timer.h"
#include "../../arm/arm.h"

void delay_ms(Timer_t* timer, uint32_t ms);

#endif