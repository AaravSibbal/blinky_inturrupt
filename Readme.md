## Diagnostic Pipeline (Zero-Heap Printf)

This module provides a robust, zero-heap `printf` diagnostic window. Standard GCC `printf` relies on dynamic memory allocation (`malloc`/`_sbrk`), which introduces the risk of heap-stack collisions in a bare-metal environment. To avoid this, this project uses a lightweight, self-contained string formatter routed directly through the ARM Cortex-M4's Instrumentation Trace Macrocell (ITM) via the SWO pin.

### Directory Structure

```text
print/
├── print.c
├── print.h
└── drivers/
    ├── dbgmcu.c
    ├── dbgmcu.h
    ├── demcr.c
    ├── demcr.h
    ├── itm.c
    ├── itm.h
    ├── tpiu.c
    └── tpiu.h
```

### File Architecture

* **`print.c` / `print.h`:** Contains the core formatting logic (via the `mpaland/printf` library) and the hardware bridge. It overrides the weak `_putchar` function to route every formatted character into the ITM driver.
* **`drivers/itm.c/h`:** The main driver for the Instrumentation Trace Macrocell. Handles unlocking the ITM, enabling global transmission, and pushing raw characters out of Stimulus Port 0.
* **`drivers/demcr.c/h`:** Controls the Debug Exception and Monitor Control Register. Responsible for enabling the master trace clock.
* **`drivers/dbgmcu.c/h`:** Controls the MCU Debug Component. Handles routing the trace signals out to the physical STM32 pins (Configured for Asynchronous mode).
* **`drivers/tpiu.c/h`:** Trace Port Interface Unit definitions. *(Note: Manual TPIU baud-rate configuration is intentionally bypassed in the C code, allowing the OpenOCD debugger to dynamically calculate and inject the correct prescaler to match the core clock.)*

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
