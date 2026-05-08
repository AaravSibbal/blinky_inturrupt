#include "exti.h"

struct EXTI{
    __IO uint32_t EXTI_IMR;
    __IO uint32_t EXTI_EMR;
    __IO uint32_t EXTI_RTSR;
    __IO uint32_t EXTI_FTSR;
    __IO uint32_t EXTI_SWIER;
    __IO uint32_t EXTI_PR;
};

#define EXTI_ENGINE ((EXTI_t *)(EXTI_BASE))

EXTI_t* EXTI_init(void){
    return EXTI_ENGINE;
}

void EXTI_set_IMR(EXTI_Line_t line){
    if(line >22){
        return;
    }
    EXTI_ENGINE->EXTI_IMR |= (1<<line);
}

void EXTI_set_RTSR(EXTI_Line_t line){
    if(line >22){
        return;
    }

    EXTI_ENGINE->EXTI_RTSR |= (1<<line);
}

void EXTI_clear_PR(EXTI_Line_t line){
    if(line>22){
        return;
    }
    EXTI_ENGINE->EXTI_PR = (1<<line);
}
