#include "systick.h"

struct Systick{
    __IO uint32_t ctrl;
    __IO uint32_t load;
    __IO uint32_t val;
    __IO uint32_t calib;
};

#define SYSTICK_BASE_ADDRESS ((uint32_t) 0xE000E010)
#define CTRL_INTER_EN_MSK (1UL<<1UL)
#define MAX_RELOAD_VAL (0x00FFFFFFUL)
#define RESET_VAL (0x01UL)
#define CLKSOURCE_BIT (2UL)
#define CLKSOURCE_SET_MSK (1UL<<CLKSOURCE_BIT)
#define CTRL_COUNTER_EN_BIT (0)
#define CTRL_COUNTER_EN_MSK (1UL<<CTRL_COUNTER_EN_BIT)

Systick_t* Systick_init(){
    return ((Systick_t *) (SYSTICK_BASE_ADDRESS));
}

static inline void Systick_enable_interrupt(Systick_t self){
    self->ctrl |= CTRL_INTER_EN_MSK;
}

static inline void Systick_set_reload_val(Systick_t self, uint32_t clock_speed_HZ){
    // example I get 16,000,000
    uint32_t reload_value = (clock_speed_HZ/1000) - 1;
    reload_value &= MAX_RELOAD_VAL;
    self->load = reload_value;
}

static inline void Systick_reset_counter(Systick_t self){
    self->val = RESET_VAL;
}

static inline void Systick_start_clock(Systick_t self){
    self->ctrl &= ~(CTRL_COUNTER_EN_MSK)
    self->ctrl |= CTRL_COUNTER_EN_MSK;
}

inline void Systick_select_clock_src(Systick_t self, Clck_src_t clck_src){
    self->ctrl &= ~(CLKSOURCE_SET_MSK);
    if(clck_src == AHB){
        self->ctrl |= (CLKSOURCE_SET_MSK);   
    }
}

void Systick_begin(Systick_t self, uint32_t clck_speed_hz, Clck_src_t clck_src){
    Systick_set_reload_val(self, clock_speed_hz);
    Systick_reset_counter(self);
    Systick_select_clock_src(self, clck_src);
    Systick_enable_interrupt(self);
    Systick_start_clock(self);
}