/* RISC-V Linker Script for QEMU 'virt' Machine
 *
 * Defines memory layout for kernel sections and stack placement.
 *
 * QEMU virt Memory Map (Reference: QEMU hw/riscv/virt.c):
 *   0x80000000 - RAM start (128 MB default)
 *   Code and data are loaded here by QEMU
 *
 * Section Layout:
 *   .text   - Executable code (read-only)
 *   .rodata - Read-only data (constants, strings)
 *   .data   - Initialized writable data
 *   .bss    - Uninitialized data (zeroed at load)
 *   stack   - 64 KB stack at end
 *
 * Documentation:
 * - GNU LD Manual: https://sourceware.org/binutils/docs/ld/
 * - ELF Specification for section types
 */

OUTPUT_ARCH(riscv)
OUTPUT_FORMAT(elf32-littleriscv)
ENTRY(_start)

SECTIONS
{
    /* Location counter starts at RAM base
     * QEMU loads kernel at 0x80000000 and begins execution at _start
     */
    . = 0x80000000;

    /* Code section - executable, read-only
     * _start must be first (ensured by link order in Makefile)
     */
    .text : {
        *(.text)
        *(.text.*)
    }

    /* Read-only data - constants and string literals */
    .rodata : {
        *(.rodata)
        *(.rodata.*)
    }

    /* Initialized data - global/static variables with initial values
     * Copied from ELF file to RAM at load time
     */
    .data : {
        *(.data)
        *(.data.*)
    }

    /* Uninitialized data - zeroed at load time
     * Doesn't consume space in ELF file, only in memory
     */
    .bss : {
        *(.bss)
        *(.bss.*)
        *(COMMON)
    }

    /* Stack grows downward from stack_top
     * 64 KB should be sufficient for most kernel operations
     * Adjust size if needed for deeply nested calls or large local arrays
     */
    . = ALIGN(8);
    . += 0x10000;
    stack_top = .;
}
