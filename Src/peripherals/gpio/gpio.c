#include "gpio.h"
#include "assert.h"

#define GPIO_BASE_ADDRESS (0x40020000)
#define GPIO_OFFSET (0x400)

typedef struct GPIO_driver{
    __IO uint32_t MODER;
    __IO uint32_t OTYPER;
    __IO uint32_t OSPEEDR;
    __IO uint32_t PUPDR;
    __IO uint32_t IDR;
    __IO uint32_t ODR;
    __IO uint32_t BSSR;
    __IO uint32_t LCKR;
    __IO uint32_t AFR[2];
} GPIO_driver_t;

struct GPIO{
    GPIO_driver_t* driver;
    GPIO_Pin_t pin;
};

void GPIO_set_moder(GPIO_t* self, GPIO_MODER_t mode){
    assert(self != NULL);
    self->driver->MODER &= ~(3UL << (2*self->pin));
    self->driver->MODER |= ((uint32_t)mode <<(2*self->pin));
}

void GPIO_set_otyper(GPIO_t* self, GPIO_OTYPER_t type){
    self->driver->OTYPER &= ~(1<<self->pin);
    self->driver->OTYPER |= (type<<self->pin);
}

void GPIO_set_odr(GPIO_t* self, GPIO_ODR_t output){
    self->driver->ODR &= ~(1<<self->pin);
    self->driver->ODR |= (output<<self->pin);
}

void GPIO_set_alt_func(GPIO_t* self, GPIO_AFx_t function){
    if(self->pin <= 7){
        self->driver->AFR[0] &= ~(15<<(self->pin*4));
        self->driver->AFR[0] |= (function<<(self->pin*4));
        return;
    }
    uint32_t temp_pin = self->pin - 8;
    self->driver->AFR[1] &= ~(15<<(temp_pin*4));
    self->driver->AFR[1] |= (function<<(temp_pin*4));
}

void GPIO_set_bssr(GPIO_t* self, BSSR_value_t val){
    if(val == BSSR_SET){
        self->driver->BSSR |= (1<<self->pin);
        return;
    }
    self->driver->BSSR |= (1<<(self->pin+16));
}

void GPIO_set_pupdr(GPIO_t *self, PUPDR_t val){
    self->driver->PUPDR &= ~(3<<(self->pin*2));
    self->driver->PUPDR |= (val<<(self->pin*2));
}

GPIO_t* GPIO_init(GPIO_t* self, GPIO_port_t port, GPIO_Pin_t pin){
    self->driver = ((GPIO_driver_t*)(GPIO_BASE_ADDRESS + (port * GPIO_OFFSET)));
    self->pin = pin;
    return self;
}