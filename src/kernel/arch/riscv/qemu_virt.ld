OUTPUT_ARCH(riscv)
ENTRY(_entry)

SECTIONS
{
    . = 0x80000000;

    .text : {
        KEEP(*(.text.entry))
        *(.text .text.*)
    }

    .rodata : {
        *(.rodata .rodata.*)
    }

    .data : {
        *(.data .data.*)
    }

    .bss : {
        *(.bss .bss.*)
        *(COMMON)
    }

    PROVIDE(end = .);
}
