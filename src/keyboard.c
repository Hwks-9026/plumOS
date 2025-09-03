// src/keyboard.c
#include "ports.h"
#include "kernel_screen.h"

static uint8_t shift_pressed = 0;
// basic US QWERTY keymap. 0 indicates no character.
const unsigned char kbdus[128] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0,
    '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, '*', 0,
    ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    '-', 0, 0, 0, '+', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};
// basic US QWERTY keymap, shift pressed. 0 indicates no character.
const unsigned char kbdus_shift[128] = {
    0,  27, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b',
    '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n',
    0, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~', 0,
    '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 0, '*', 0,
    ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    '-', 0, 0, 0, '+', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};
// This is the function called by our assembly ISR
void irq_handler() {
    unsigned char scancode = inb(0x60);

    // Check for Shift key presses
    if (scancode == 0x2A || scancode == 0x36) {
        shift_pressed = 1;
    }
    // Check for Shift key releases
    else if (scancode == 0xAA || scancode == 0xB6) {
        shift_pressed = 0;
    }
    // Handle a regular key press
    else if (scancode < 128) {
        char c;
        // Use the correct keymap based on the shift state
        if (shift_pressed) {
            c = kbdus_shift[scancode];
        } else {
            c = kbdus[scancode];
        }

        // If it's a printable character, print it
        if (c != 0) {
            print_char(c);
			update_cursor();
        }
    }

    // Send End-of-Interrupt signal to the PIC
    outb(0x20, 0x20);
}


