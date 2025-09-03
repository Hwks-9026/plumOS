AS = i686-elf-as
CC = i686-elf-gcc
LD = i686-elf-ld

GR = grub-mkrescue

# Add -masm=intel to CFLAGS for the inline assembly syntax
CFLAGS = -O3 -Wall -Wextra -ffreestanding -mno-red-zone -m32 -masm=intel
LDFLAGS = -T src/link.ld -nostdlib

OBJS = .build/boot.o \
       .build/gdt_s.o \
       .build/idt_s.o \
       .build/gdt_c.o \
       .build/idt_c.o \
	   .build/timer.o \
       .build/keyboard.o \
       .build/kernel_screen.o \
       .build/kernel_init.o

TARGET = iso/boot/kernel.elf

all: plum.iso

plum.iso: $(TARGET)
	$(GR) -o plum.iso iso/

$(TARGET): $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $^

.build/boot.o: src/boot.s
	$(AS) -o $@ $<

.build/gdt_s.o: src/gdt.s
	$(AS) -o $@ $<

.build/idt_s.o: src/idt.s
	nasm -f elf -o $@ $<

.build/gdt_c.o: src/gdt.c
	$(CC) $(CFLAGS) -c -o $@ $<

.build/idt_c.o: src/idt.c
	$(CC) $(CFLAGS) -c -o $@ $<

.build/timer.o: src/timer.c
	$(CC) $(CFLAGS) -c -o $@ $<

.build/keyboard.o: src/keyboard.c
	$(CC) $(CFLAGS) -c -o $@ $<

.build/kernel_screen.o: src/kernel_screen.c
	$(CC) $(CFLAGS) -c -o $@ $<

.build/kernel_init.o: src/kernel_init.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf .build/*.o iso/boot/kernel.elf plum.iso

run: plum.iso
	qemu-system-i386 -cdrom plum.iso
