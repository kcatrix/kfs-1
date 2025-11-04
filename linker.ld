ENTRY(start)

SECTIONS
{
    . = 0x100000;            /* Adresse de chargement du kernel */

    .text :
    {
        *(.multiboot)
        *(.text*)
    }

    .rodata : { *(.rodata*) }
    .data   : { *(.data*) }
    .bss    : { *(.bss*) }
}
