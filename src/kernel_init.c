#include <stdint.h>

void print(const char* str); // declare the external function

void kernel_main(void) {
    print("Hello from print!\n");
    print("but what about second print statement");
    while (1) __asm__ __volatile__("hlt");
}

