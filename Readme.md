## Diagnostic Pipeline (Zero-Heap Printf)

This module provides a robust, zero-heap `printf` diagnostic window. Standard GCC `printf` relies on dynamic memory allocation (`malloc`/`_sbrk`), which introduces the risk of heap-stack collisions in a bare-metal environment. To avoid this, this project uses a lightweight, self-contained string formatter routed directly through the ARM Cortex-M4's Instrumentation Trace Macrocell (ITM) via the SWO pin.

### Internal Hardware Requirements

To successfully route a character from the ARM core to the VS Code terminal, several specific hardware gates must be opened in sequence during boot:

1. **Power the Trace Clock:** Bit 24 (`TRCENA`) in the `DEMCR` register must be set to `1`. Without this, the ITM is unpowered and ignores all writes.
2. **Enable ITM Transmission:** In the ITM Trace Control Register (`ITM_TCR`), both Bit 0 (`ITMENA` - Master Enable) and Bit 3 (`TXENA` - TX Enable) must be flipped on.
3. **Open Stimulus Port 0:** The ITM has 32 ports. Port 0 is enabled via the Trace Enable Register (`ITM_TER`) to provide a dedicated channel for `printf` data.
4. **Link the Formatter:** The `print.c` library requires a `_putchar(char c)` function. This function takes the character and writes it to the ITM Port 0 register.

### How to Use

Because the `print` module bypasses the standard C library, you must include its specific header. Once included, it behaves exactly like the standard `printf`, but operates entirely on the local stack.

**Standard Printing (To VS Code Console):**
```c
#include "print.h"

int main(void) {
    // Hardware initialization required first
    ITM_init(); 

    int status_code = 200;
    printf("Bare-metal boot successful. Status: %d\n", status_code);
}
```

# Interrupt Management Service

## Overview
The Interrupt Management Service provides a unified, bare-metal hardware abstraction for handling exceptions and interrupts on the ARM Cortex-M architecture. It abstracts the underlying routing between system-level exceptions and device-specific peripheral interrupts, allowing the application layer to manage all interrupts through a single, cohesive API.

## Directory Structure
The service is decoupled into high-level service logic and low-level peripheral drivers:
*   `services/interrupts/`: Contains the primary application-facing API and logic.
*   `peripherals/nvic/`: Driver layer for the Nested Vectored Interrupt Controller (device-specific interrupts).
*   `peripherals/scb/`: Driver layer for the System Control Block (core system exceptions).

## Architecture Notes

### Unified Routing (NVIC vs. SCB)
The Cortex-M architecture splits interrupt management. System exceptions (such as Non-Maskable Interrupts, HardFaults, and SVCalls) are managed by the System Control Block (SCB) and possess negative `IRQn_t` values. Device-specific peripheral interrupts (like I2C, SPI, or Timers) are managed by the NVIC and possess positive `IRQn_t` values.

This service automatically routes API calls to the correct underlying driver (`nvic` or `scb`) by evaluating the sign of the provided `IRQn_t`. The user does not need to manually differentiate between core exceptions and peripheral interrupts when setting priorities or enabling IRQs.

### SysTick Exception
The SysTick timer is a special case within the ARM architecture. While it is a system exception (and typically carries a negative IRQn), its control and status registers are mapped to its own dedicated peripheral space, distinct from standard SCB priority registers. This service handles SysTick internally as an architectural exception, ensuring priority assignments and state queries route to the correct SysTick registers seamlessly.

## Fault Handling & Debugging
The service implements robust fault handlers for system crashes:
*   `HardFault_Handler`
*   `MemManage_Handler`
*   `BusFault_Handler`
*   `UsageFault_Handler`

**Dependency:** These handlers heavily rely on the **Print Service**. 
When a fault occurs, the processor is trapped in an infinite loop. Before halting, the fault handler extracts the CPU stack frame (R0-R3, R12, LR, PC, and xPSR) and the relevant Fault Status Registers (FSR). The Print Service is strictly required to output this register dump via the debug interface (e.g., UART or SWO) to diagnose memory violations, bus errors, or unaligned access issues.

## API Reference

### Configuration
*   `void set_priority_grouping(PriorityGroup_t pg);`
    Configures the split between preemption priority and subpriority bits. This must be called during system initialization before configuring individual interrupt priorities.

### State Management
*   `void enable_IRQ(IRQn_t IRQn);`
    Enables the specified interrupt or exception.
*   `void disable_IRQ(IRQn_t IRQn);`
    Disables the specified interrupt or exception.
*   `void set_pending_IRQ(IRQn_t IRQn);`
    Forces an interrupt into a pending state via software.
*   `void clear_pending_IRQ(IRQn_t IRQn);`
    Clears the pending state of an interrupt.
*   `uint32_t get_pending_IRQ(IRQn_t IRQn);`
    Returns `1` if the specified interrupt is currently pending, `0` otherwise.
*   `uint32_t get_active(IRQn_t IRQn);`
    Returns `1` if the specified interrupt is currently active (executing), `0` otherwise.

### Priority Management
*   `void set_priority(IRQn_t IRQn, uint32_t priority);`
    Assigns a preemption/subpriority value to the given interrupt. The service automatically routes this to the NVIC, SCB, or SysTick registers based on the `IRQn`.
*   `uint32_t get_priority(IRQn_t IRQn);`
    Reads back the current priority level of the specified interrupt.


## Interrupt Service Routines (ISRs)

### Vector Table Implementation
This framework relies on a statically defined vector table located in the startup assembly file. All hardware interrupt handlers are initially defined using weak aliases (`[WEAK]`) that point to a default infinite loop (typically `Default_Handler`).

### Overriding Handlers
To service a specific interrupt, you must override its weak alias by defining a function with the exact hardware-defined name in your application code. The linker will automatically replace the default handler with your custom implementation during the build process.

**Example: I2C1 Event Interrupt**
To handle an I2C event, define the corresponding ISR in your source file:

```c
void I2C1_EV_IRQHandler(void) {
    // 1. Verify and clear the specific interrupt status flag
    // 2. Execute peripheral logic
}
```

### Important Implementation Notes

*   **Decoupled Architecture:** There is no runtime function registration API in this service. The Interrupt Management Service strictly handles the *configuration* (enabling, disabling, prioritizing) via the SCB and NVIC. The hardware directly handles the *execution* via the vector table.
*   **Symbol Matching:** The function name in your C code must match the symbol name in the startup assembly file exactly. If there is a typo, the compiler will not throw an error, but the interrupt will fall back to the `Default_Handler` and trap the CPU.
