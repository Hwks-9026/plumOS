#![no_std] // don't link the Rust standard library
#![no_main] // disable all Rust-level entry points
extern crate core;
static HELLO: &[u8] = b"Hello World!";
use core::panic::PanicInfo;

/// This function is called on panic.
#[panic_handler]
fn panic(_info: &PanicInfo) -> ! {
    loop {}
}

#[unsafe(no_mangle)] // don't mangle the name of this function
pub extern "C" fn _start() -> ! {
    let vga_buffer = 0xb8000 as *mut u8;
    for (i, &byte) in HELLO.iter().enumerate() {
        unsafe {
            *vga_buffer.offset(i as isize * 2) = byte;
            *vga_buffer.offset(i as isize * 2 + 1) = 0xb;
        }
    }
    // this function is the entry point, since the linker looks for a function
    // named `_start` by default
    loop {}
}

#[repr(C, align(8))]
struct MultibootHeader {
    magic: u32,
    architecture: u32,
    header_length: u32,
    checksum: u32,
    end_tag_type: u16,
    end_tag_flags: u16,
    end_tag_size: u32,
}

#[unsafe(link_section = ".multiboot2")]
#[used]
static MULTIBOOT_HEADER: MultibootHeader = MultibootHeader {
    magic: 0xE85250D6,
    architecture: 0,
    header_length: 24,
    checksum: 0x100000000u64
        .wrapping_sub(0xE85250D6 + 0 + 24) as u32,
    end_tag_type: 0,
    end_tag_flags: 0,
    end_tag_size: 8,
};

