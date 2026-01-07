#include "../include/kernel/kernel.h"
#include "../console/console.h"

/* kprint - Output string to console and update screen buffer */
void kprintf(const char *str) {
        // TODO: implement a proper kprint with formatting
        console_puts(str);
}

/* kprint_newline - Advance to next line in screen buffer */
static void kprint_newline(void) {
        console_putc('\n');
}