.set MAGIC, 0x1BADB002
.set FLAGS, 0x00
.set CHECKSUM, -(MAGIC + FLAGS)

.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

# Reserve 16KB for the stack
.set STACK_SIZE, 0x4000

.section .bss
.align 16
stack_bottom:
.skip STACK_SIZE
stack_top:

.section .text
.global _start
.extern kernel_main

_start:
    # Set up the stack pointer. The stack grows downwards.
    mov $stack_top, %esp

    # Call the C entry point
    call kernel_main

    # Hang the CPU if kernel_main returns
    cli
.hang:
    hlt
    jmp .hang
