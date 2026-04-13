#ifndef NVIC_H
#define NVIC_H

#include "def.h"

#define NVIC_BASE (0xE000E100)

typedef int32_t IRQn_t;
#define EXTI0_IRQ ((IRQn_t)6)

typedef struct nvic{
    __IO uint32_t NVIC_ISER[8];
    uint32_t RESERVED_0[24];
    __IO uint32_t NVIC_ICER[8];
    uint32_t RESERVED_1[24];
    __IO uint32_t NVIC_ISPR[8];
    uint32_t RESERVED_2[24];
    __IO uint32_t NVIC_ICPR[8];
    uint32_t RESERVED_3[24];
    __IO uint32_t NVIC_IABR[8];
    uint32_t RESERVED_4[56];
    __IO uint8_t NVIC_IPR[240];
} nvic_t;

#define NVIC_ENGINE ((nvic_t*) NVIC_BASE)



void NVIC_enable_IRQ(IRQn_t IRQn);
void NVIC_disable_IRQ(IRQn_t IRQn);
void NVIC_set_pending_IRQ(IRQn_t IRQn);
void NVIC_clear_pending_IRQ(IRQn_t IRQn);
uint32_t  NVIC_get_pending_IRQ(IRQn_t IRQn);
uint32_t NVIC_get_active(IRQn_t IRQn);
void NVIC_set_priority(IRQn_t IRQn, uint32_t priority);
void NVIC_get_priority(IRQn_t IRQn, uint32_t priority);


#endif