// src/keyboard.c
#include "keyboard.h"
#include "ports.h"
#include "kernel_screen.h"

// A basic US QWERTY keymap. 0 indicates no character.
const unsigned char kbdus[128] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0,
    '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, '*', 0,
    ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    '-', 0, 0, 0, '+', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

// This is the function called by our assembly ISR
void irq_handler() {
    unsigned char scancode = inb(0x60);

    // Check if it's a key press (top bit is 0)
    if (scancode < 128) {
        char c = kbdus[scancode];
        if (c != 0) {
            print_char(c);
        }
    }

    // Send End-of-Interrupt signal to the PIC
    outb(0x20, 0x20);
}

void keyboard_init() {
    print("Keyboard initialized.\n");
}
