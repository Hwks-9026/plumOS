// src/ports.h
#ifndef PORTS_H
#define PORTS_H

#include <stdint.h>

// Read a byte from a port
static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    // The 'in' instruction syntax is 'in dest, src' which matches this.
    __asm__ __volatile__("in %0, %1" : "=a"(ret) : "Nd"(port));
    return ret;
}

// Write a byte to a port
static inline void outb(uint16_t port, uint8_t data) {
    // The 'out' instruction syntax is 'out dest, src', so we flip the operands.
    __asm__ __volatile__("out %1, %0" : : "a"(data), "Nd"(port));
}

#endif // PORTS_H
