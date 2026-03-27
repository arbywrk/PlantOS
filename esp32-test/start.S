    .section .entry, "ax"
    .globl _start
    .globl __stack_top
    .globl __bss_start
    .globl __bss_end

_start:
    /* Set stack pointer */
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
    call    early_init
    call    main

3:
    j       3b
