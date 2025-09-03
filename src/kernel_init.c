// src/kernel_init.c
#include <stdint.h>
#include "kernel_screen.h"
#include "gdt.h"
#include "idt.h"          // Include the new IDT header

void kernel_main(void) {
	clear_screen();
    gdt_init();
    idt_init();       // Initialize the IDT after the GDT

    // Enable interrupts!
    __asm__ __volatile__ ("sti");


	// The rest of the kernel will now be driven by interrupts.
    // We can hang here.
    while (1) {
        __asm__ __volatile__("hlt");
    }
}
