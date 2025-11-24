#!/bin/bash

riscv64-unknown-elf-gcc -march=rv32im_zicsr -mabi=ilp32 -nostdlib -c bootloader/bootloader.S -o ../build/bootloader.o
riscv64-unknown-elf-gcc -march=rv32im_zicsr -mabi=ilp32 -c uart/uart.c -o ../build/uart.o
riscv64-unknown-elf-gcc -march=rv32im_zicsr -mabi=ilp32 -c kernel/kmain.c -o ../build/kmain.o
riscv64-unknown-elf-gcc -O0 -march=rv32im_zicsr -mabi=ilp32 -nostdlib -T linker/linker.ld ../build/bootloader.o ../build/kmain.o ../build/uart.o -o ../build/bootloader.elf

# riscv64-unknown-elf-gcc -march=rv32im_zicsr -mabi=ilp32 -nostdlib -c bootloader/bootloader.S -o ../build/bootloader.o
# riscv64-unknown-elf-gcc -march=rv32im_zicsr -mabi=ilp32 -nostdlib -T linker/linker.ld ../build/bootloader.o -o ../build/bootloader.elf

qemu-system-riscv32 -machine virt -m 4G -bios opensbi-riscv32-generic-fw_dynamic.bin -kernel ../build/bootloader.elf