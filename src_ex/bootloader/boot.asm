org 0x7C00 ; all addresses start from this offset
bits 16 ; tell the assembler to produce 16 bit instructions

%define ENDL 0x0D, 0x0A ; nasm macro for endline

#
# FAT12 Header
#
jmp short start
nop

bdb_oem                    db 'MSWIN4.1' ; 8 bytes
bdb_bytes_per_second       dw 512
bdb_sectors_per_cluter     db 1 
bdb_reserved_sectors       dw 1
bdb_fat_count              db 2
bdb_dir_entries_count      dw 0xE0
bdb_total_sectors          dw 2880       ; 2880 * 512 = 1.44MB
bdb_media_description_type db 0xF0       ; F0 = 3.5" floppy disk
bdb_sectors_per_fat        dw 9          ; 9 sectors/fat


start:
  jmp main


;
; Print a string to the screen
; Param:
;   - ds:si pointer to string
;
puts:
  ; save registers
  push si
  push ax

.loop:
  lodsb        ; load next character in al
  or al, al    ; verify if next chaacter is null
  jz .done     ; finish if that is the case
  
  ; calling the bios interrupt for printing
  mov ah, 0x0E ; signal for print to screen 
  xor bh, bh   ; page number
  int 0x10     ; video interupt

  jmp .loop    ; loop 

.done:
  pop ax
  pop si
  ret

main:
  ; setup data segment
  mov ax, 0 ; can't write to ds/ex directly
  mov ds, ax
  mov es, ax

  ; setup stack
  mov ss, ax
  mov sp, 0x7C00 ; the stack will grow 'downwards'. To not let it override the program put it at the begining

  ; print hello world
  mov si, msg_hello
  call puts

  hlt ; make the cpu halt

.halt:
  jmp .halt ; in case the above command does not work set up an infinit loop

msg_hello: db "Hello RamOS!", ENDL, 0

times 510-($-$$) db 0
dw 0x0AA55
