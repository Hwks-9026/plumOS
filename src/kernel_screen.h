#ifndef KERNEL_SCREEN_H
#define KERNEL_SCREEN_H

void clear_screen();
void print(const char* msg);
void print_char(char c);
void update_cursor();
#endif // KERNEL_SCREEN_H
