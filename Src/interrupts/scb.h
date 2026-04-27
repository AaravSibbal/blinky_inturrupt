#ifndef SCB_H
#define SCB_H

#include "../def.h"

#define SCB_BASE (0xE000ED00)
#define BUSFAULTENA_MASK (1UL << 17)

typedef struct SCB{
    __IO uint32_t SCB_CPUID;
    __IO uint32_t SCB_ICSR;
    __IO uint32_t SCB_VTOR;
    __IO uint32_t SCB_AIRCR;
    __IO uint32_t SCB_SCR;
    __IO uint32_t SCB_CCR;
    __IO uint32_t SCB_SHPR[3];
    __IO uint32_t SCB_SHSCR;
    __IO uint32_t SCB_CFSR;
    __IO uint32_t SCB_HFSR;
    __IO uint32_t SCB_MMAR;
    __IO uint32_t SCB_BFAR;
    __IO uint32_t SCB_AFSR;
} SCB_t;

#define SCB_ENGINE ((SCB_t*) SCB_BASE)

void SCB_en_bus_fault(void);
void BusFault_Handler(void);



#endif