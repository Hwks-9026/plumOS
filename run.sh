#!/bin/bash
cd os
cargo build -r
cd ..
cd root
cp ../os/target/x86_64-os/release/plumOS ./plumOS.elf
grub-mkrescue -o ../plum.iso .
cd ..
qemu-system-x86_64 -cdrom plum.iso
