section .multiboot
align 4
    dd 0x1BADB002       ; magic number
    dd 0x00              ; flags
    dd -(0x1BADB002 + 0x00)

section .note.GNU-stack noalloc noexec nowrite progbits

section .text
global _start
_start:
    extern main
    call main
    cli
hang:
    hlt
    jmp hang
