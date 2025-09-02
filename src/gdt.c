#include "gdt.h"
// A GDT entry (segment descriptor)
#include <stdint.h>

struct gdt_entry {
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t  base_middle;
    uint8_t  access;
    uint8_t  granularity;
    uint8_t  base_high;
} __attribute__((packed));

// The GDT pointer passed to the lgdt instruction
struct gdt_ptr {
    uint16_t limit; // Size of the GDT
    uint32_t base;  // Starting address of the GDT
} __attribute__((packed));

// Our GDT will have 3 entries: Null, Kernel Code, Kernel Data
struct gdt_entry gdt[3];
struct gdt_ptr gp;

// External assembly function to load the GDT
extern void gdt_flush(uint32_t);

// Helper function to set a GDT entry
void gdt_set_gate(int num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran) {
    gdt[num].base_low    = (base & 0xFFFF);
    gdt[num].base_middle = (base >> 16) & 0xFF;
    gdt[num].base_high   = (base >> 24) & 0xFF;

    gdt[num].limit_low   = (limit & 0xFFFF);
    gdt[num].granularity = (limit >> 16) & 0x0F;

    gdt[num].granularity |= gran & 0xF0;
    gdt[num].access      = access;
}

// Main GDT initialization function
void gdt_init() {
    // Set up the GDT pointer
    gp.limit = (sizeof(struct gdt_entry) * 3) - 1;
    gp.base  = (uint32_t)&gdt;

    // 1. Null segment (required)
    gdt_set_gate(0, 0, 0, 0, 0);

    // 2. Kernel Code Segment
    // Base: 0x0, Limit: 4GB, Access: Ring 0, Read/Execute
    // Granularity: 4KB blocks, 32-bit protected mode
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);

    // 3. Kernel Data Segment
    // Base: 0x0, Limit: 4GB, Access: Ring 0, Read/Write
    // Granularity: 4KB blocks, 32-bit protected mode
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);

    // Load the GDT
    gdt_flush((uint32_t)&gp);
}
