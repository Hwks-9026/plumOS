#!/bin/bash
cd ./rust_boot_experiments/
cargo build -r
cd ..
cd rust_experiment_root
cp ../rust_boot_experiments/target/x86_64-os/release/plumOS ./plumOS.elf
grub-mkrescue -o ../plum.iso .
cd ..
qemu-system-x86_64 -cdrom plum.iso
