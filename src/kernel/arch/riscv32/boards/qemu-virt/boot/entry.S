# entry.S - Reset entry for the QEMU virt machine.
#
# The startup structure is intentionally minimal:
# - establish gp
# - build an early stack
# - jump into a tiny C start() routine
# That keeps early bring-up easy to reason about while the kernel is small.

.section .text.entry

.global _entry

_entry:
  .option push
  .option norelax
  la gp, __global_pointer$
  .option pop

  la sp, stack0
  li a0, 4096
  csrr a1, mhartid
  addi a1, a1, 1
  mul a0, a0, a1
  add sp, sp, a0

  call start

halt_loop:
  wfi
  j halt_loop
