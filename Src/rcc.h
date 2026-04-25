#ifndef RCC_H
#define RCC_H

#include "def.h"

#define RCC_BASE (0x40023800)
#define CCR_OFFSET (0x00)
#define AHB1_ENR_OFFSET (0x30)
#define APB2_ENR_OFFSET (0x44)

#define RCC_CCR (RCC_BASE + CCR_OFFSET)
#define RCC_AHB1_ENR (*(__IO uint32_t *)((RCC_BASE) + (AHB1_ENR_OFFSET))) 
#define RCC_APB2_ENR (*(__IO uint32_t *)((RCC_BASE) + (APB2_ENR_OFFSET)))

#define GPIOA_BIT 0
#define GPIOD_BIT 3
#define SYSCFG_BIT 14
#define RCC_AHB1_GPIOD_EN (1<<GPIOD_BIT)
#define RCC_AHB1_GPIOA_EN (1<<GPIOA_BIT)
#define RCC_APB2_SYSCFG_EN (1<<SYSCFG_BIT)

#endif