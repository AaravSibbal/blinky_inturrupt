#ifndef SYSTICK_H
#define SYSTICK_H

#include "../../def.h"

typedef struct Systick* Systick_t;

typedef enum CLCK_SRC{
    AHB_BY_8 = 0,
    AHB = 1
} Clck_src_t;

Systick_t Systick_init(void);
void Systick_begin(Systick_t self, uint32_t clck_speed_hz, Clck_src_t clck_src);
void Systick_select_clock_src(Systick_t self, Clck_src_t clck_src);
void SysTick_Handler(void);


#endif