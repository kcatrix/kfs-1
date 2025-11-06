ENTRY(_start)

SECTIONS {
    . = 1M;

    .text ALIGN(4K) : {
        *(.multiboot)
        *(.text)
    } :code

    .rodata ALIGN(4K) : {
        *(.rodata*)
    } :rodata

    .data ALIGN(4K) : {
        *(.data)
    } :data

    .bss ALIGN(4K) : {
        *(COMMON)
        *(.bss)
    } :data

    /DISCARD/ : {
        *(.eh_frame)
        *(.note.GNU-stack)
    }
}

PHDRS {
    code PT_LOAD FLAGS(5);    /* 5 = RE */
    rodata PT_LOAD FLAGS(4);  /* 4 = R */
    data PT_LOAD FLAGS(6);    /* 6 = RW */
}
