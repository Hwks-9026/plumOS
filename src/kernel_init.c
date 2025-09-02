// src/kernel_init.c
#include <stdint.h>
#include "kernel_screen.h"
#include "gdt.h"
#include "idt.h"          // Include the new IDT header
#include "keyboard.h"     // Include the new keyboard header

void kernel_main(void) {
    gdt_init();
    idt_init();       // Initialize the IDT after the GDT
    keyboard_init();  // Initialize the keyboard

    print("PlumOS is running!\n");
    print("Keyboard input is enabled.\n");

    // Enable interrupts!
    __asm__ __volatile__ ("sti");

    // The rest of your kernel will now be driven by interrupts.
    // We can hang here.
    while (1) {
        __asm__ __volatile__("hlt");
    }
}
