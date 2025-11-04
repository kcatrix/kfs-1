; boot/boot.s
section .multiboot
    align 4
    dd 0x1BADB002            ; magic number Multiboot
    dd 0x00                  ; flags
    dd -(0x1BADB002 + 0x00)  ; checksum

section .text
global start
extern kernel_main

start:
    ; configure la pile
    mov esp, stack_top

    ; appelle la fonction principale du kernel (en C)
    call kernel_main

.hang:
    hlt
    jmp .hang

section .bss
align 16
stack_bottom:
    resb 4096                 ; 4 Ko de pile
stack_top:
