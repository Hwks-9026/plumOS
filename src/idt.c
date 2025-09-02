// src/idt.c
#include "idt.h"
#include "ports.h"
#include "kernel_screen.h"

// IDT entry and pointer structures
struct idt_entry { uint16_t base_low; uint16_t selector; uint8_t always0; uint8_t flags; uint16_t base_high; } __attribute__((packed));
struct idt_ptr { uint16_t limit; uint32_t base; } __attribute__((packed));

struct idt_entry idt[256];
struct idt_ptr   idtp;

// --- External Assembly Functions ---
extern void idt_load(uint32_t);
extern void exception_isr(); // New exception stub
extern void generic_isr();   // Generic hardware IRQ stub
extern void isr1();          // Specific keyboard IRQ stub

// --- C-Level Interrupt Handlers ---
// C handler for CPU exceptions. This function will not return.
void exception_handler() {
    print("CPU Exception Triggered! System Halted.\n");
    // Halt the CPU
    while(1) {
        __asm__ __volatile__ ("cli; hlt");
    }
}

// Generic handler for hardware interrupts
void generic_irq_handler() {
    outb(0x20, 0x20); // EOI to master PIC
}

// Helper to set an IDT entry
void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags) {
    idt[num].base_low  = (base & 0xFFFF);
    idt[num].base_high = (base >> 16) & 0xFFFF;
    idt[num].selector  = sel;
    idt[num].always0   = 0;
    idt[num].flags     = flags;
}

// Remap the PIC
void pic_remap() {
    outb(0x20, 0x11); outb(0xA0, 0x11); outb(0x21, 0x20); outb(0xA1, 0x28);
    outb(0x21, 0x04); outb(0xA1, 0x02); outb(0x21, 0x01); outb(0xA1, 0x01);
    outb(0x21, 0x0);  outb(0xA1, 0x0);
}

// Main IDT initialization
void idt_init() {
    idtp.limit = (sizeof(struct idt_entry) * 256) - 1;
    idtp.base  = (uint32_t)&idt;

    pic_remap();

    // --- Populate the IDT ---
    // 1. Point all CPU exceptions (0-31) to the exception ISR
    for (int i = 0; i < 32; i++) {
        idt_set_gate(i, (uint32_t)exception_isr, 0x08, 0x8E);
    }

    // 2. Point all hardware interrupts to the generic IRQ ISR
    for (int i = 32; i < 256; i++) {
        idt_set_gate(i, (uint32_t)generic_isr, 0x08, 0x8E);
    }
    
    // 3. Overwrite the keyboard entry with its specific ISR
    idt_set_gate(33, (uint32_t)isr1, 0x08, 0x8E);

    // Load the new IDT
    idt_load((uint32_t)&idtp);
}
