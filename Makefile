all: kernel.iso

boot/boot.o: boot/boot.s
	nasm -f elf32 boot/boot.s -o boot/boot.o

kernel/main.o: kernel/main.c
	gcc -m32 -ffreestanding -fno-builtin -fno-stack-protector -nostdlib -nostartfiles -nodefaultlibs -c -o kernel/main.o kernel/main.c

kernel/screen.o: kernel/screen.c
	gcc -m32 -ffreestanding -fno-builtin -fno-stack-protector -nostdlib -nostartfiles -nodefaultlibs -c -o kernel/screen.o kernel/screen.c

kernel/ports.o: kernel/ports.c
	gcc -m32 -ffreestanding -fno-builtin -fno-stack-protector -nostdlib -nostartfiles -nodefaultlibs -c -o kernel/ports.o kernel/ports.c

kernel/keyboard.o: kernel/keyboard.c
	gcc -m32 -ffreestanding -fno-builtin -fno-stack-protector -nostdlib -nostartfiles -nodefaultlibs -c -o kernel/keyboard.o kernel/keyboard.c

kernel.bin: boot/boot.o kernel/main.o kernel/screen.o kernel/ports.o kernel/keyboard.o
	ld -m elf_i386 -T linker.ld -o kernel.bin boot/boot.o kernel/main.o kernel/screen.o kernel/ports.o kernel/keyboard.o

kernel.iso: kernel.bin
	mkdir -p iso/boot/grub
	cp kernel.bin iso/boot/
	echo 'set timeout=0' > iso/boot/grub/grub.cfg
	echo 'set default=0' >> iso/boot/grub/grub.cfg
	echo 'menuentry "KFS1" { multiboot /boot/kernel.bin }' >> iso/boot/grub/grub.cfg
	grub-mkrescue -o kernel.iso iso/

run:
	qemu-system-i386 -cdrom kernel.iso

clean:
	rm -rf *.o *.bin iso boot/*.o kernel/*.o kernel.bin kernel.iso
