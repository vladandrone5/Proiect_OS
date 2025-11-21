#!/bin/bash

#riscv64-unknown-elf-gcc -march=rv32ima -mabi=ilp32 -nostdlib -c bootloader.S -o bootloader.o
#riscv64-unknown-elf-gcc -march=rv32ima -mabi=ilp32 -nostdlib -T linker.ld bootloader.o -o bootloader.elf

riscv64-unknown-elf-gcc -march=rv32im_zicsr -mabi=ilp32 -nostdlib -c bootloader.S -o bootloader.o
riscv64-unknown-elf-gcc -march=rv32im_zicsr -mabi=ilp32 -c kmain.c -o kmain.o
riscv64-unknown-elf-gcc -O0 -march=rv32im_zicsr -mabi=ilp32 -nostdlib -T linker.ld bootloader.o kmain.o -o bootloader.elf


qemu-system-riscv32 -machine virt -m 4G -bios opensbi-riscv32-generic-fw_dynamic.bin -kernel bootloader.elf
