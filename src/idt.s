; Externs for our C-level handlers
extern irq_handler
extern generic_irq_handler
extern exception_handler
extern timer_handler

section .text

; --- Stub for CPU exceptions ---
global exception_isr
exception_isr:
    call exception_handler
    hlt

; --- Stubs for Hardware Interrupts (IRQs) ---
; Generic, catch-all ISR for unhandled hardware interrupts
global generic_isr
generic_isr:
    pushad
    call generic_irq_handler
    popad
    iretd

; Corrected macro definition: specifies it takes 2 arguments
%macro IRQ_ISR 2
    global isr%1
isr%1:
    pushad
    call %2 ; Call the specific C handler
    popad
    iretd
%endmacro

; Create the ISRs using the macro
IRQ_ISR 0, timer_handler    ; For the PIT (IRQ 0 -> int 32)
IRQ_ISR 1, irq_handler      ; For the Keyboard (IRQ 1 -> int 33)

; Function to load the IDT
global idt_load
idt_load:
    mov eax, [esp+4]
    lidt [eax]
    ret
