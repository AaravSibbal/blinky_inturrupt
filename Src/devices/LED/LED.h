#ifndef LED_H
#define LED_H

#include "../../def.h"
#include "../../peripherals/gpio/gpio.h"

typedef struct LED LED_t;

typdef enum LED_color{
    GREEN = 12,
    ORANGE,
    RED,
    BLUE
}LED_color_t;

LED_t* LED_init(LED_t* self, LED_color_t color);
void toggle_LED(LED_t* self);

#endif