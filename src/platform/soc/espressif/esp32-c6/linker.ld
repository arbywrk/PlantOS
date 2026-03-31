/* linker.ld - ESP32-C6 linker script for PlantOS.
 *
 * Responsibilities:
 * - define the memory map used by the final image
 * - place sections at the correct runtime addresses
 * - export linker symbols used by startup code
 */
ENTRY(_start)

MEMORY
{
    RAM (rwx) : ORIGIN = 0x40800000, LENGTH = 512K
}

SECTIONS
{
    . = ORIGIN(RAM);

    .text :
    {
        KEEP(*(.entry))
        *(.text*)
        *(.rodata*)
        . = ALIGN(16);
    } > RAM

    .data :
    {
        . = ALIGN(16);
        __data_start = .;
        *(.data*)
        . = ALIGN(16);
        __data_end = .;
    } > RAM

    .bss (NOLOAD) :
    {
        . = ALIGN(16);
        __bss_start = .;
        *(.bss*)
        *(COMMON)
        . = ALIGN(16);
        __bss_end = .;
    } > RAM

    .stack (NOLOAD) :
    {
        . = ALIGN(16);
        __stack_bottom = .;
        . += 0x4000;    /* 16 KiB */
        . = ALIGN(16);
        __stack_top = .;
    } > RAM

    /DISCARD/ :
    {
        *(.comment)
        *(.note*)
        *(.eh_frame*)
    }
}
