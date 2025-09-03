// src/timer.c
#include "timer.h"
#include "idt.h"
#include "ports.h"
#include "kernel_screen.h"

volatile uint32_t ticks = 0;

// This is the C-level handler called by the assembly ISR (isr0)
void timer_handler() {
    ticks++;
    outb(0x20, 0x20);
}

// Configure the PIT to a specific frequency
void pit_init(uint32_t frequency) {
    // The PIT's input frequency is approximately 1.193182 MHz
    uint32_t divisor = 1193182 / frequency;

    // Send the command byte to the command port
    // 0x36 sets Channel 0, Access mode: lo/hi byte, Mode 3: square wave
    outb(0x43, 0x36);

    // Send the divisor's low and high bytes to the data port
    uint8_t low  = (uint8_t)(divisor & 0xFF);
    uint8_t high = (uint8_t)((divisor >> 8) & 0xFF);
    outb(0x40, low);
    outb(0x40, high);
}
