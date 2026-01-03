bits 32 ; nasm directive

section .text
        ;multiboot spec
        align 4
        dd 0x1BADB002            ;magic
        dd 0x00                  ;flags
        dd - (0x1BADB002 + 0x00) ;checksum. m+f+c should be zero

global start
extern kmain

start: ; entry point
  cli ; (clear interupt) set the interupt flag to 0 (block interupts)
  mov esp, stack_space ; set the stack pointer (esp -> 32bit stack pointer)
  call kmain ; calling the external c function
  hlt ; halt the CPU

section .bss
  resb 8192  ; 8KB stack
  stack_space:

