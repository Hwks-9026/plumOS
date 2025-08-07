#![no_std] // don't link the Rust standard library
#![no_main] // disable all Rust-level entry points
extern crate core;
use core::panic::PanicInfo;

mod kernel;

#[panic_handler]
fn panic(_info: &PanicInfo) -> ! {
    loop {}
}

#[unsafe(no_mangle)] // don't mangle the name of this function
pub extern "C" fn _start() -> ! {
    kernel::kernel_loop();
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
    checksum: 0x100000000u64.wrapping_sub(0xE85250D6 + 0 + 24) as u32,
    end_tag_type: 0,
    end_tag_flags: 0,
    end_tag_size: 8,
};


#[derive(Debug, Clone, Copy, PartialEq, Eq)]
#[repr(u32)]
pub(crate) enum QemuExitCode {
    Success = 0x10,
    Failed = 0x11,
}

pub(crate) fn exit_qemu(exit_code: QemuExitCode) {
    use x86_64::instructions::port::Port;

    unsafe {
        let mut port = Port::new(0xf4);
        port.write(exit_code as u32);
    }
}
