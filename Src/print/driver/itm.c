#include "itm.h"
#include "dbgmcu.h"
#include "../../gpio.h"
#include "tpiu.h"

static void ITM_unlock_access(ITM_t * const self){
    self->ITM_lock_access = ITM_UNLOCK_MAGIC_WRITE; 
}

static void ITM_enable(ITM_t * const self){
    self->ITM_trace_ctrl |= ITM_GLOBAL_EN_MSK;
}

static void ITM_gpio_setup(){
    GPIO_set_moder(ITM_GPIO_PORT, ITM_GPIO_PIN, GPIO_MODE_ALT);
    GPIO_set_alt_func(ITM_GPIO_PORT, ITM_GPIO_PIN, AF0);
}

void ITM_unlock_port(ITM_t *const self, const uint8_t port){
    if(port >= 32){
        return;
    }

    uint32_t trace_priv_bit = port/4;
    self->ITM_trace_priv |= (1UL<<trace_priv_bit);
    self->ITM_trace_en |= (1UL<<port);
}

void ITM_init(ITM_t * const self){
    ITM_gpio_setup();
    DBGMCU_debug_enable(DBGMCU_ASYNC);
    TPIU_set_proto(TPIU_SWO_NRZ);
    ITM_unlock_access(self);
    ITM_enable(self);
}

void ITM_put_char(ITM_t * const self, const char * const c, 
const uint8_t port){
    if(port >= 32){
        return;
    }
    self->ITM_stim_port[port] = (uint32_t)c;
}