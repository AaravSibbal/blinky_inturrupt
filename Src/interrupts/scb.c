#include "scb.h"
#include "Src/print/printf.h"
#include <cstdio>
#include <stdint.h>

static inline void SCB_enable_errors(SCB_t * const self){
    self->SCB_SHSCR |= (BUSFAULTENA_MSK) | (USGFAULTENA_MSK) | (MEMFAULTENA_MSK);
}

static inline void SCB_en_stack_align(SCB_t * const self){
    self->SCB_CCR |= (STKALIGN_MSK);
}

static inline void SCB_en_div0_trap(SCB_t * const self){
    self->SCB_CCR |= (DIV_0_TRP_MSK);
}

void SCB_init(SCB_t * const self){
    SCB_enable_error(self);
    SCB_en_stack_align(self);
    SCB_en_div0_trap(self);
}

static inline void SCB_clr_cfsr(SCB_t * const self, uint32_t old_cfsr){
    self->SCB_CFSR = old_cfsr;
}


static void print_stack(const uint32_t * const fault_stack){
    uint32_t r0 = fault_stack[0];
    uint32_t r1 = fault_stack[1];
    uint32_t r2 = fault_stack[2];
    uint32_t r3 = fault_stack[3];
    uint32_t r12 = fault_stack[4];
    uint32_t lr = fault_stack[5];
    uint32_t pc = fault_stack[6];
    uint32_t psr = fault_stack[7];

    /**
    r0-r3: are the first four function arguments that ran
    it can be felpful to catch a null pointer, will show up
    as 0x00000000 
    r12: not very useful
    psr: bits 0-8 give the irq number for the interrupt we are in
    0 means main loop.
    pc: this is the program counter, and shows the exact point the
    program is in when the fault happens
    lr: this is the link register and given the return address
    for the current function that we are on. 
    */
    printf("r0: 0x%08lX", r0);
    printf("r1: 0x%08lX", r1);
    printf("r2: 0x%08lX", r2);
    printf("r3: 0x%08lX", r3);
    printf("r12: 0x%08lX", r12);
    printf("lr: 0x%08lX", lr);
    printf("pc: 0x%08lX", pc);
    printf("psr: 0x%08lX", psr);
}

__attribute__((naked)) void HardFault_Handler(void) {
    __asm volatile (
        "tst lr, #4 \n"         // Check which stack pointer was in use (MSP or PSP)
        "ite eq \n"             // If/Then/Else block
        "mrseq r0, msp \n"      // If MSP was used, move MSP into R0
        "mrsne r0, psp \n"      // If PSP was used, move PSP into R0
        "b HardFault_Decoder \n" // Branch to your C function, passing R0 as the first argument
    );
}


static void HardFault_decoder(const uint32_t * const fault_stack){
    printf("[HARD FAULT HANDLER]\n")
    print_stack(fault_stack);
    printf ("BFAR = 0x%08lX\n", SCB_ENGINE->SCB_BFAR);
    printf ("CFSR = 0x%08lX\n", SCB_ENGINE->SCB_CFSR);
    printf ("HFSR = 0x%08lX\n", SCB_ENGINE->SCB_HFSR);
    while(1){
        // loop forever
    }
}

__attribute__((naked)) void BusFault_Handler(void){
    __asm volatile (
        "tst lr, #4 \n"         // Check which stack pointer was in use (MSP or PSP)
        "ite eq \n"             // If/Then/Else block
        "mrseq r0, msp \n"      // If MSP was used, move MSP into R0
        "mrsne r0, psp \n"      // If PSP was used, move PSP into R0
        "b BusFault_Decoder \n" // Branch to your C function, passing R0 as the first argument       
    );
}

static void BusFault_Decoder(const uint32_t * const fault_stack){
    uint32_t cfsr = SCB_ENGINE->SCB_CFSR;
    uint32_t bfar = SCB_ENGINE->SCB_BFAR;
    printf("[BUS FAULT HANDLER]\n");
    print_stack(fault_stack);

    if(cfsr & (CFSR_IBUSERR_MSK)){
        printf("Instruction bus error. The processor detects the instruction bus error on prefetching an instruction\n");
    }
    if(cfsr & (CFSR_PRECISEERR_MSK)){
        printf("Precise error: The PC value stacked for the exception return points to the instruction that caused the fault.\n");
    }
    if(cfsr & (CFSR_IMPRECISEERR_MSK)){
        printf("Imprecise error: The return address in the stack frame is not related to the instruction that caused the error.\n");
    }
    if(cfsr & (CFSR_UNSTKERR_MSK)){
        printf("Unstack error: Unstack for an exception return has caused one or more bus faults.\n");
    }
    if(cfsr & (CFSR_STKERR_MSK)){
        printf("Stack error: Stacking for an exception entry has caused one or more bus faults\n")
    }
    if(cfsr & (CFSR_LSPERR_MSK)){
        printf("Floating point lazy state prevention error.\n");
    }
    if(cfsr & (CFSR_BFARVALID_MSK)){
        printf("We have a valid address for the fault.\n");
        printf("Address: 0x%08lX\n", bfar);
    }

    // clear cfsr
    SCB_clr_cfsr(SCB_ENGINE, cfsr);

    while(1){
        __asm volatile ("nop");
    }
}

__attribute__((naked)) void MemManage_Handler(void){
    __asm volatile (
        "tst lr, #4 \n"         // Check which stack pointer was in use (MSP or PSP)
        "ite eq \n"             // If/Then/Else block
        "mrseq r0, msp \n"      // If MSP was used, move MSP into R0
        "mrsne r0, psp \n"      // If PSP was used, move PSP into R0
        "b MemManage_Decoder \n" // Branch to your C function, passing R0 as the first argument       
    );
}

static void MemManage_Decoder(const uint32_t * const fault_stack){
    uint32_t mmfar = SCB_ENGINE->SCB_MMFAR;
    uint32_t cfsr = SCB_ENGINE->SCB_CFSR;
    printf("[MEMORY MANAGEMENT FAULT DETECTED]\n")
    print_stack(fault_stack);
    if(cfsr & (CFSR_IACCVIOL_MSK)){
        printf("Instruction access violation: The processor attempted an instruction fetch from a location that does not permit execution.\n");
    }
    if(cfsr & (CFSR_DACCVIOL_MSK)){
        printf("Data access violation: The processor attempted a load or store at a location that does not permit the operation.\n");
    }
    if(cfsr & (CFSR_MUNSTKERR_MSK)){
        printf("Unstacking error on a return from exception: Unstack for an exception return has caused one or more access violations.\n");
    }
    if(cfsr & (CFSR_MSTKERR_MSK)){
        printf("Memory manager fault on stacking for exception entry: Stacking for an exception entry has caused one or more access violations.\n");
    }   
    if(cfsr & (CFSR_MLSPERR_MSK)){
        printf("Floating point lazy prevention error: A MemManage fault occurred during floating-point lazy state preservation\n");
    }
    if(cfsr & (CFSR_MMARVALID_MSK)){
        printf("We found a valid memory address for the fault.\n");
        printf("Address: 0x%08lX", mmfar);
    }

    SCB_clr_cfsr(SCB_ENGINE, cfsr);
    while(1){
        __asm volatile ("nop");
    }
}