#include "../../../param.h"

void kmain();

__attribute__((aligned(16))) char stack0[4096 * MAX_HARTS];

void start(void) {
        // set M Previous Privilege mode to Supervisor, for mret.
        // ...

        // set M Exception Program Counter to main, for mret.
        // requires gcc -mcmodel=medany
        // ...

        // disable paging for now.
        // ...

        // delegate all interrupts and exceptions to supervisor mode.
        // ...

        // configure Physical Memory Protection to give supervisor mode
        // access to all of physical memory.
        // ...

        // ask for clock interrupts.
        // ...

        // keep each CPU's hartid in its tp register, for cpuid().
        // ...

        // switch to supervisor mode and jump to main().
        // ...

        kmain();
}
