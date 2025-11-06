# Compilateurs
ASM = nasm
CC = gcc
LD = ld

# Flags de compilation C (sans les flags C++)
CFLAGS = -m32 -ffreestanding -fno-builtin -fno-stack-protector -fno-exceptions \
         -nostdlib -nostartfiles -nodefaultlibs \
         -fno-unwind-tables -fno-asynchronous-unwind-tables \
         -Wall -Wextra -Werror

# Flags ASM
ASMFLAGS = -f elf32

# Flags du linker
LDFLAGS = -m elf_i386 -T linker.ld

# Fichiers objets
BOOT_OBJ = boot/boot.o
KERNEL_OBJS = kernel/main.o kernel/screen.o kernel/ports.o kernel/keyboard.o

# Cible par défaut
all: kernel.iso

# Compilation du boot ASM
boot/boot.o: boot/boot.s
	$(ASM) $(ASMFLAGS) boot/boot.s -o boot/boot.o

# Compilation des fichiers C du kernel
kernel/main.o: kernel/main.c kernel/screen.h kernel/keyboard.h
	$(CC) $(CFLAGS) -c -o kernel/main.o kernel/main.c

kernel/screen.o: kernel/screen.c kernel/screen.h kernel/ports.h
	$(CC) $(CFLAGS) -c -o kernel/screen.o kernel/screen.c

kernel/ports.o: kernel/ports.c kernel/ports.h
	$(CC) $(CFLAGS) -c -o kernel/ports.o kernel/ports.c

kernel/keyboard.o: kernel/keyboard.c kernel/keyboard.h kernel/ports.h kernel/screen.h
	$(CC) $(CFLAGS) -c -o kernel/keyboard.o kernel/keyboard.c

# Linkage du kernel
kernel.bin: $(BOOT_OBJ) $(KERNEL_OBJS)
	$(LD) $(LDFLAGS) -o kernel.bin $(BOOT_OBJ) $(KERNEL_OBJS)

# Création de l'ISO bootable
kernel.iso: kernel.bin
	mkdir -p iso/boot/grub
	cp kernel.bin iso/boot/
	echo 'set timeout=0' > iso/boot/grub/grub.cfg
	echo 'set default=0' >> iso/boot/grub/grub.cfg
	echo 'menuentry "KFS1" { multiboot /boot/kernel.bin }' >> iso/boot/grub/grub.cfg
	grub-mkrescue -o kernel.iso iso/

# Exécution avec QEMU
run: kernel.iso
	qemu-system-i386 -cdrom kernel.iso

# Exécution avec KVM (plus rapide)
run-kvm: kernel.iso
	qemu-system-i386 -enable-kvm -cdrom kernel.iso

# Debug avec QEMU
debug: kernel.iso
	qemu-system-i386 -cdrom kernel.iso -s -S

# Nettoyage
clean:
	rm -rf boot/*.o kernel/*.o

fclean: clean
	rm -rf kernel.bin kernel.iso iso/

re: fclean all

.PHONY: all run run-kvm debug clean fclean re

git: 
	git add .
	git commit -m "auto commit"
	git push