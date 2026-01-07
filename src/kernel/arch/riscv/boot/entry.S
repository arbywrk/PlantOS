# credit to xv6 (riscv version) they where a big help
# https://github.com/mit-pdos/xv6-riscv

# qemu -kernel loads the kernel at 0x80000000
# and causes each hart (i.e. CPU) to jump there.
# kernel.ld causes the following code to
# be placed at 0x80000000.
.section .text.entry

.global _entry

_entry:
  # set up a stack for C.
  # stack0 is declared in start.c,
  # with a 4096-byte stack per CPU.
  # sp = stack0 + ((hartid + 1) * 4096)
  la sp, stack0    # sp = &stack0
  li a0, 4096      # a0 = 4906
  csrr a1, mhartid # a1 = mhartid
  addi a1, a1, 1   # a1 = a1 + 1
  mul a0, a0, a1   # a0 = a0 * a1
  add sp, sp, a0   # sp = sp + a0

  # jump to start() in start.c
  call start

# If start returns, enter fallback low-power loop
halt_loop:
    wfi              # Wait For Interrupt - sleep until interrupt
    j halt_loop
