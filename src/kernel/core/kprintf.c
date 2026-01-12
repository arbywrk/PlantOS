#include "../include/kernel/types.h"
#include "../include/kernel/kernel.h"
#include "../console/console.h"

#include "../lib/kstdarg.h"

static char digits[] = "0123456789abcdef";

/**
 * @brief Prints an integer 'xx' in base 'base' interpreted as 
 *        signed if 'sign' is 1 and as unsigned if it is 0.
 * 
 * @param xx Integer to print
 * @param base Base of the integer
 * @param sign Boolean telling if the integer is signed
 */
static void printint(int xx, int base, int sign) {
        char buf[20]; // 64 bits ~ 20 decimal digits
        int i = 0;
        int x;

        if (sign && xx < 0) {
                sign = 1;
                x = -xx;
        } else {
                x = xx;
                sign = 0;
        }

        do {
                buf[i++] = digits[x % base];
        } while((x /= base) != 0);

        if (sign) {
                buf[i++] = '-';
        }

        while (--i >= 0) {
                console_putc(buf[i]);
        }

}

/**
 * @brief Output string to console and update scrren buffer
 * 
 * @param str Format string
 * @param ... Arguments for the format string
 * 
 * WARNING: For the moment the only implemented formats are:
 * 
 *          - %d and %i
 * 
 *          - %u
 * 
 *          - %x
 * 
 *          - %%
 */
int kprintf(const char *str, ...) {
        __attribute__((format(printf, 1, 2)));
        va_list ptr;
        va_start(ptr, str);

        for (int i = 0; str[i] != 0; ++i) {
                if (str[i] != '%') {
                        console_putc(str[i]);
                        continue;
                }

                char next_ch = str[++i];
                switch (next_ch) {
                        case '%':
                                console_putc('%');
                                break;
                        case 'd':
                        case 'i':
                                printint(va_arg(ptr, int), 10, 1);
                                break;
                        case 'u':
                                printint(va_arg(ptr, int), 10, 1);
                                break;
                        case 'x':
                                printint(va_arg(ptr, int), 16, 0);
                                break;
                                
                }
        }
        return 0;
}

/* kprint_newline - Advance to next line in screen buffer */
static void kprint_newline(void) {
        console_putc('\n');
}