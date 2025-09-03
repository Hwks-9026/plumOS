// src/kernel_init.c
#include <stdint.h>
#include "kernel_screen.h"
#include "gdt.h"
#include "idt.h"          // Include the new IDT header
// src/kernel_init.c
#include "timer.h" // Include the new timer header
// ... (other includes)

extern volatile uint32_t ticks; // Get the tick counter from timer.c

void kernel_main(void) {
    gdt_init();
    idt_init();
    pit_init(100); // Initialize timer to 100 Hz
	clear_screen();
    print("PlumOS is running!\n");

    __asm__ __volatile__ ("sti");

    while (1) {
		__asm__ __volatile__ ("hlt");
    }
}
