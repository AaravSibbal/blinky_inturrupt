#include "nvic.h"

static void size_check(IRQn_t IRQn){
    if(IRQn < 0||  IRQn>81){
        // NOTE: print here when I have it
        return;
    }
}

void NVIC_enable_IRQ(IRQn_t IRQn){
    size_check(IRQn);
    uint32_t iser_idx = IRQn/32;
    uint32_t iser_bit = IRQn%32;

    NVIC_ENGINE->NVIC_ISER[iser_idx] = (1UL<<iser_bit);
}

void NVIC_disable_IRQ(IRQn_t IRQn){
    size_check(IRQn);
    uint32_t icer_idx = IRQn/32;
    uint32_t icer_bit = IRQn%32;

    NVIC_ENGINE->NVIC_ICER[icer_idx] = (1UL<<icer_bit);
}

void NVIC_set_pending_IRQ(IRQn_t IRQn){
    size_check(IRQn);
    uint32_t ispr_idx = IRQn/32;
    uint32_t ispr_bit = IRQn%32;

    NVIC_ENGINE->NVIC_ISPR[ispr_idx] = (1UL<<ispr_bit);
}

void NVIC_clear_pending_IRQ(IRQn_t IRQn){
    size_check(IRQn);
    uint32_t icpr_idx = IRQn/32;
    uint32_t icpr_bit = IRQn%32;

    NVIC_ENGINE->NVIC_ICPR[icpr_idx] = (1UL<<icpr_bit);
}

uint32_t NVIC_get_pending_IRQ(IRQn_t IRQn){
    return 0;
}