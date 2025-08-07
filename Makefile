AS = i686-elf-as
CC = i686-elf-gcc
LD = i686-elf-ld

GR = grub-mkrescue

CFLAGS = -O2 -Wall -Wextra -ffreestanding -mno-red-zone -m32
LDFLAGS = -T src/link.ld -nostdlib

all: plum.iso

plum.iso: iso/kernel.elf
	$(GR) -o plum.iso iso/

.build/boot.o: src/boot.s
	$(AS) -o $@ $<

.build/kernel_screen.o: src/kernel_screen.c
	$(CC) $(CFLAGS) -c -o $@ $<

.build/kernel_init.o: src/kernel_init.c
	$(CC) $(CFLAGS) -c -o $@ $<

iso/kernel.elf: .build/boot.o .build/kernel_init.o .build/kernel_screen.o
	$(LD) $(LDFLAGS) -o $@ $^

clean:
	rm -f .build/*.o iso/kernel.elf plum.iso

run: plum.iso
	qemu-system-x86_64 -cdrom plum.iso

