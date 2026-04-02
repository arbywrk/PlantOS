/*
 * start.S - Generic RISC-V early entry point.
 *
 * Responsibilities:
 * - provide the linked entry symbol `_start`
 * - establish the initial stack
 * - clear the `.bss` section
 * - hand off to the first platform-specific C startup function
 *
 * Called by:
 * - the platform boot chain after the image entry point is reached
 */
.section .entry, "ax"
.globl _start
.globl platform_start
.globl __stack_top
.globl __bss_start
.globl __bss_end

_start:
    /* Set up stack */
    la      sp, __stack_top

    /* Zero .bss */
    la      a0, __bss_start
    la      a1, __bss_end
1:
    beq     a0, a1, 2f
    sw      zero, 0(a0)
    addi    a0, a0, 4
    j       1b

2:
    /* Handoff to platform-specific early C startup */
    call    platform_start

3:
    j       3b
