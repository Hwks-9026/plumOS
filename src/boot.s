.section .multiboot
.align 4
.long 0x1BADB002         # magic number
.long 0x00               # flags
.long -(0x1BADB002 + 0x00) # checksum (magic + flags + checksum == 0)

.section .text
.global _start
.extern kernel_main

_start:
    call kernel_main
    cli
.hang:
	hlt
	jmp .hang

