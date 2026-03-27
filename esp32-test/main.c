#include <stdint.h>
#include <stdbool.h>
#include "esp_rom_gpio.h"

int esp_rom_printf(const char *fmt, ...);

#define SIG_GPIO_OUT_IDX 128u
#define GPIO_NUM 3u

#define GPIO_OUT_W1TS_REG (*(volatile uint32_t *) 0x60091008u)
#define GPIO_OUT_W1TC_REG (*(volatile uint32_t *) 0x6009100Cu)
#define GPIO_ENABLE_W1TS_REG (*(volatile uint32_t *) 0x60091024u)

/* LP watchdog registers */
#define LP_WDT_BASE 0x600B1C00u
#define LP_WDT_CONFIG0_REG (*(volatile uint32_t *) (LP_WDT_BASE + 0x00u))
#define LP_WDT_FEED_REG (*(volatile uint32_t *) (LP_WDT_BASE + 0x14u))
#define LP_WDT_WPROTECT_REG (*(volatile uint32_t *) (LP_WDT_BASE + 0x18u))
#define LP_WDT_WKEY 0x50D83AA1u

static inline void gpio_output_enable(uint32_t gpio_num) {
        GPIO_ENABLE_W1TS_REG = (1u << gpio_num);
}

static inline void gpio_set_high(uint32_t gpio_num) {
        GPIO_OUT_W1TS_REG = (1u << gpio_num);
}

static inline void gpio_set_low(uint32_t gpio_num) {
        GPIO_OUT_W1TC_REG = (1u << gpio_num);
}

static void delay(volatile uint32_t count) {
        while (count--) {
                __asm__ volatile("nop");
        }
}

void early_init(void) {
        /* Disable LP watchdog early */
        LP_WDT_WPROTECT_REG = LP_WDT_WKEY;
        uint32_t cfg = LP_WDT_CONFIG0_REG;
        cfg &= ~((1u << 31) | (1u << 12)); /* clear enable and flashboot bits */
        LP_WDT_CONFIG0_REG = cfg;
        LP_WDT_FEED_REG = 1u;
        LP_WDT_WPROTECT_REG = 0u;
}

int main(void) {
        esp_rom_printf("\r\nboot: entered main()\r\n");
        esp_rom_printf("boot: configuring GPIO%u\r\n", GPIO_NUM);

        esp_rom_gpio_pad_select_gpio(GPIO_NUM);
        esp_rom_gpio_connect_out_signal(GPIO_NUM, SIG_GPIO_OUT_IDX, false, false);
        gpio_output_enable(GPIO_NUM);

        for (;;) {
                esp_rom_printf("boot: HIGH\r\n");
                gpio_set_high(GPIO_NUM);
                delay(30000000);

                esp_rom_printf("boot: LOW\r\n");
                gpio_set_low(GPIO_NUM);
                delay(30000000);
        }
}
