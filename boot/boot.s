section .multiboot
align 4
    dd 0x1BADB002       ; magic number
    dd 0x00              ; flags
    dd -(0x1BADB002 + 0x00)

section .text
global _start
_start:
    extern kmain
    call kmain
    cli
hang:
    hlt
    jmp hang
