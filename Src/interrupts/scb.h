#ifndef SCB_H
#define SCB_H

#include "../def.h"
#include "../print/printf.h"

#define SCB_BASE (0xE000ED00)

#define USGFAULTENA_MSK (1UL << 18)
#define BUSFAULTENA_MSK (1UL << 17)
#define MEMFAULTENA_MSK (1UL << 16)
#define STKALIGN_MSK (1UL<<9)
#define DIV_0_TRP_MSK (1UL<<4)

// BUS FAULT ERR MSKS
#define CFSR_BFARVALID_MSK (1UL<<15)
#define CFSR_LSPERR_MSK (1UL<<13)
#define CFSR_STKERR_MSK (1UL<<12)
#define CFSR_UNSTKERR_MSK (1UL<<11)
#define CFSR_IMPRECISEERR_MSK (1UL<<10)
#define CFSR_PRECISEERR_MSK (1UL<<9)
#define CFSR_IBUSERR_MSK (1UL<<8)

// MEMORY MANAGEMENT ERR MSKS
#define CFSR_IACCVIOL_MSK (1UL<<0)
#define CFSR_DACCVIOL_MSK (1UL<<1)
#define CFSR_MUNSTKERR_MSK (1UL<<3)
#define CFSR_MSTKERR_MSK (1UL<<4)
#define CFSR_MLSPERR_MSK (1UL<<5)
#define CFSR_MMARVALID_MSK (1UL<<7)

// USAGE FAULTS ERR MSKS
#define CFSR_UNDEFINSTR_MSK (1UL<<16)
#define CFSR_INVSTATE_MSK (1UL<17)
#define CFSR_INVPC_MSK (1UL<<18)
#define CFSR_NOCP_MSK (1UL<<19)
#define CFSR_UNALIGNED_MSK (1UL<<24)
#define CFSR_DIVBYZERO_MSK (1UL<<25)


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

/**
    enables bus, mem and usage faults
*/
void SCB_init(SCB_t * const self);

void HardFault_Handler(void);

void BusFault_Handler(void);
void MemManage_Handler(void);
void UsageFault_Handler(void);

#endif