use core::{arch::asm, i32};
static HELLO: &[u8] = b"Hello World!";

pub(crate) fn kernel_loop() -> ! {
    let vga_buffer = 0xb8000 as *mut u8;
    loop{
        for (i, &byte) in HELLO.iter().enumerate() {
            unsafe {
                *vga_buffer.offset(i as isize * 2) = byte;
                *vga_buffer.offset(i as isize * 2 + 1) = 0xb;
            }
        }
        sleep();
        for (i, &_byte) in HELLO.iter().enumerate() {
            unsafe {
                *vga_buffer.offset(i as isize * 2) = b' ';
                *vga_buffer.offset(i as isize * 2 + 1) = 0xb;
            }
        }
        sleep();
    }
}


fn sleep() {

    for _ in 0..250000000 {
        // Or inline assembly if needed
        unsafe {
            asm!(
                "nop"
            )
        }
    }
}
