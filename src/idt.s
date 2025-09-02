; src/idt.s

; Externs for our C-level handlers
extern irq_handler          ; Specific handler for the keyboard
extern generic_irq_handler  ; Generic handler for hardware interrupts
extern exception_handler    ; New handler for CPU exceptions

section .text

; --- Stub for CPU exceptions ---
; All CPU exceptions (0-31) will be routed here.
global exception_isr
exception_isr:
    ; Note: We don't save registers because we are not going to return.
    ; We will just halt the system.
    call exception_handler
    ; The C handler will not return, it will hang the CPU.
    hlt

; --- Stubs for Hardware Interrupts (IRQs) ---
; Generic, catch-all ISR for unhandled hardware interrupts
global generic_isr
generic_isr:
    pushad
    call generic_irq_handler
    popad
    iretd

; Specific ISR for the keyboard
global isr1
isr1:
    pushad
    call irq_handler
    popad
    iretd

; Function to load the IDT
global idt_load
idt_load:
    mov eax, [esp+4]
    lidt [eax]
    ret
