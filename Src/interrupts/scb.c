#include "scb.h"

void SCB_en_bus_fault(){
    SCB_ENGINE->SCB_SHSCR |= BUSFAULTENA_MASK;
}

void BusFault_Handler(){
    
}