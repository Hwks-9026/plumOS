use core::arch::asm;
static HELLO: &[u8] = b"Hello World!";

pub(crate) fn kernel_loop() -> ! {
    let vga_buffer = 0xb8000 as *mut u8;
    loop {
        for (i, &byte) in HELLO.iter().enumerate() {
            unsafe {
                *vga_buffer.offset(i as isize * 2) = byte;
                *vga_buffer.offset(i as isize * 2 + 1) = 0xF;
            }
        }
        blocking_test_sleep();
        for (i, &_byte) in HELLO.iter().enumerate() {
            unsafe {
                *vga_buffer.offset(i as isize * 2) = b' ';
                *vga_buffer.offset(i as isize * 2 + 1) = 0xF;
            }
        }
        blocking_test_sleep();
    }
}

//sleeps about a second on qemu on my machine
fn blocking_test_sleep() {
    for _ in 0..250000000 {
        unsafe { asm!("nop") }
    }
}
