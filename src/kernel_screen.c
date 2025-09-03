#include "kernel_screen.h"
#include <stdint.h>
#include "ports.h"

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_ADDRESS 0xB8000
#define COLOR 0x0F  // white on black

static uint16_t* const VGA_MEMORY = (uint16_t*)VGA_ADDRESS;
static uint8_t cursor_row = 0;
static uint8_t cursor_col = 0;


static void scroll() {
    if (cursor_row < VGA_HEIGHT) return;

    // Move all rows up by one
    for (int row = 1; row < VGA_HEIGHT; row++) {
        for (int col = 0; col < VGA_WIDTH; col++) {
            VGA_MEMORY[(row - 1) * VGA_WIDTH + col] = VGA_MEMORY[row * VGA_WIDTH + col];
        }
    }

    // Clear last row
    for (int col = 0; col < VGA_WIDTH; col++) {
        VGA_MEMORY[(VGA_HEIGHT - 1) * VGA_WIDTH + col] = ((uint16_t)COLOR << 8) | ' ';
    }

    cursor_row = VGA_HEIGHT - 1;
}

void clear_screen() {
    for (int row = 0; row < VGA_HEIGHT; row++) {
        for (int col = 0; col < VGA_WIDTH; col++) {
			VGA_MEMORY[row * VGA_WIDTH + col] = 0;
		}
	}
}

void print_char(char c) {
    if (c == '\n') {
        cursor_col = 0;
        cursor_row++;
        scroll();
        return;
    }
	else if (c == '\b') {
		if (cursor_col > 0) {
			cursor_col--;
			VGA_MEMORY[cursor_row * VGA_WIDTH + cursor_col] = ((uint16_t)COLOR << 8) | ' ';
		}
		return;
	}
	else if (c == '\t') {
		cursor_col += 4;
		return;
	}

    VGA_MEMORY[cursor_row * VGA_WIDTH + cursor_col] = ((uint16_t)COLOR << 8) | c;
    cursor_col++;

    if (cursor_col >= VGA_WIDTH) {
        cursor_col = 0;
        cursor_row++;
        scroll();
    }
}

void update_cursor() {
	uint16_t pos = cursor_row * VGA_WIDTH + cursor_col;

    // Send the high byte of the position
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));

    // Send the low byte of the position
    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t)(pos & 0xFF));
}


void print(const char* msg) {
    for (int i = 0; msg[i] != '\0'; i++) {
        print_char(msg[i]);
    }
	update_cursor();
}

