#include "scb.h"

void SCB_en_bus_fault(void){
    SCB_ENGINE->SCB_SHSCR |= BUSFAULTENA_MASK;
}

void BusFault_Handler(void){
    
}