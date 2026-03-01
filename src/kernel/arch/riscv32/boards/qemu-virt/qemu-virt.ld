/* qemu-virt.ld - Linker script for the QEMU virt machine.
 *
 * QEMU's -kernel flow places the image at 0x80000000 in DRAM. The layout keeps
 * startup simple: direct-mapped code/data, explicit BSS clearing, and a stack
 * that lives outside the zeroed section.
 */

OUTPUT_ARCH(riscv)
ENTRY(_entry)

PHDRS
{
    text PT_LOAD FLAGS(5);
    data PT_LOAD FLAGS(6);
}

SECTIONS
{
    . = 0x80000000;

    .text : {
        KEEP(*(.text.entry))
        *(.text .text.*)
    } :text

    .rodata : {
        *(.rodata .rodata.*)
    } :text

    .data : {
        _data_start = .;
        *(.data .data.*)
        _data_end = .;
    } :data

    .bss : {
        _bss_start = .;
        *(.bss .bss.*)
        *(COMMON)
        _bss_end = .;
    } :data

    .noinit (NOLOAD) : {
        *(.noinit .noinit.*)
        *(.noinit.stack)
    } :data

    PROVIDE(__global_pointer$ = _data_start + 0x800);
    PROVIDE(end = .);
}
