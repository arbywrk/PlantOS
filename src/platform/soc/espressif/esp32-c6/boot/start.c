#include <kernel/platform.h>

extern void kmain(void);

void platform_start(void) {
        platform_early_init();
        kmain();

        for (;;) {
                platform_idle();
        }
}
