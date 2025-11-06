ENTRY(_start)                      # Indique au linker que le point d'entrée du programme est la fonction _start (définie dans boot.s)

SECTIONS { 

  . = 1M;                          # Place le début du kernel à l’adresse 1 Mo (0x00100000), où GRUB charge le kernel

  .text : { *(.multiboot) *(.text) }  # Section du code exécutable : inclut d’abord le header multiboot, puis tout le code machine (.text)

  .rodata : { *(.rodata*) }        # Section des données en lecture seule (constantes, chaînes de caractères, etc.)

  .data : { *(.data) }             # Section des données initialisées en lecture/écriture (ex : int x = 10;)

  .bss : { *(.bss) }               # Section des données non initialisées (ex : int compteur;), qui seront mises à zéro au démarrage

}                           
