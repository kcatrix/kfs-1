ASM     = nasm
CC      = gcc
LD      = ld
CFLAGS  = -m32 -ffreestanding -fno-pic -fno-builtin -nostdlib -nostartfiles -nodefaultlibs -Wall -Wextra
ASFLAGS = -f elf32

SRC_ASM = boot/boot.s
SRC_C   = src/kernel.c
OBJ     = boot.o kernel.o

all: kernel.bin

boot.o: $(SRC_ASM)
	$(ASM) $(ASFLAGS) -o $@ $<

kernel.o: $(SRC_C)
	$(CC) $(CFLAGS) -c -o $@ $<

kernel.bin: $(OBJ) linker.ld
	$(LD) -m elf_i386 -T linker.ld -o $@ $(OBJ)

clean:
	rm -f *.o boot/*.o src/*.o kernel.bin

run: all
	qemu-system-i386 -kernel kernel.bin
