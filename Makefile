ASM = nasm
CC = gcc
LD = ld
CFLAGS = -m32 -ffreestanding -fno-builtin -fno-stack-protector -nostdlib -nostartfiles -nodefaultlibs

all: kernel.iso

boot/boot.o: boot/boot.s
	$(ASM) -f elf32 $< -o $@

kernel/kernel.o: kernel/main.c kernel/screen.c
	$(CC) $(CFLAGS) -m32 -c kernel/main.c -o kernel/main.o
	$(CC) $(CFLAGS) -m32 -c kernel/screen.c -o kernel/screen.o

kernel.bin: boot/boot.o kernel/main.o kernel/screen.o linker.ld
	$(LD) -m elf_i386 -T linker.ld -o kernel.bin boot/boot.o kernel/main.o kernel/screen.o

kernel.iso: kernel.bin
	mkdir -p iso/boot/grub
	cp kernel.bin iso/boot/
	echo 'set timeout=0' > iso/boot/grub/grub.cfg
	echo 'set default=0' >> iso/boot/grub/grub.cfg
	echo 'menuentry "KFS1" { multiboot /boot/kernel.bin }' >> iso/boot/grub/grub.cfg
	grub-mkrescue -o kernel.iso iso/

run: kernel.iso
	qemu-system-i386 -cdrom kernel.iso

clean:
	rm -rf *.o *.bin iso boot/*.o kernel/*.o kernel.bin kernel.iso
