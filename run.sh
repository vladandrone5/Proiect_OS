#!/bin/bash

mkdir -p ../build
echo "Compiling and linking RISC-V OS components..."

# Define the cross-compiler and target architecture
CC="riscv64-unknown-elf-gcc"
ARCH="-march=rv32im_zicsr -mabi=ilp32"
COMMON_FLAGS="-nostdlib -ffreestanding -O0 -Wall -Wextra"
LINKER_SCRIPT="linker/linker.ld"
OUTPUT_ELF="../build/bootloader.elf"

# --- Compilation of C and Assembly Files ---

# Bootloader Assembly
$CC $ARCH -c bootloader/bootloader.S -o ../build/bootloader.o

# C Modules (Assuming you have these paths and files)
$CC $ARCH $COMMON_FLAGS -c uart/uart.c -o ../build/uart.o
$CC $ARCH $COMMON_FLAGS -c uart/print.c -o ../build/print.o
$CC $ARCH $COMMON_FLAGS -c timer/timer.c -o ../build/timer.o
$CC $ARCH $COMMON_FLAGS -c memory/memory.c -o ../build/memory.o
$CC $ARCH $COMMON_FLAGS -c syscall/syscall.c -o ../build/syscall.o
$CC $ARCH $COMMON_FLAGS -c plic/plic.c -o ../build/plic.o
$CC $ARCH $COMMON_FLAGS -c string/string.c -o ../build/string.o
$CC $ARCH $COMMON_FLAGS -c process/load_context.S -o ../build/load_context.o
$CC $ARCH $COMMON_FLAGS -c process/switch_context.S -o ../build/switch_context.o
$CC $ARCH $COMMON_FLAGS -c process/process.c -o ../build/process.o
$CC $ARCH $COMMON_FLAGS -c example_progs/prog.c -o ../build/progs.o
$CC $ARCH $COMMON_FLAGS -c terminal/sys_cmd.c -o ../build/sys_cmd.o
$CC $ARCH $COMMON_FLAGS -c keyboard/keyboard.c -o ../build/keyboard.o
$CC $ARCH $COMMON_FLAGS -c interrupts/interrupts.c -o ../build/interrupts.o
$CC $ARCH $COMMON_FLAGS -c kernel/kmain.c -o ../build/kmain.o

# --- Linking ---
# Link all object files into the final executable
$CC $ARCH $COMMON_FLAGS -T $LINKER_SCRIPT -o $OUTPUT_ELF \
    ../build/bootloader.o \
    ../build/syscall.o \
    ../build/uart.o \
    ../build/print.o \
    ../build/timer.o \
    ../build/memory.o \
    ../build/plic.o \
    ../build/string.o \
    ../build/load_context.o \
    ../build/switch_context.o \
    ../build/process.o \
    ../build/progs.o \
    ../build/sys_cmd.o \
    ../build/keyboard.o \
    ../build/interrupts.o \
    ../build/kmain.o

# Check if linking was successful
if [ $? -ne 0 ]; then
    echo "Linking failed. Exiting."
    exit 1
fi

echo "Linking successful: $OUTPUT_ELF created."

# --- Run on QEMU ---
echo "Starting QEMU..."

# Convert ELF to binary for QEMU (QEMU can load ELF directly, but this is a common step for other loaders)
riscv64-unknown-elf-objcopy -O binary $OUTPUT_ELF ../build/bootloader.bin

# Run QEMU
qemu-system-riscv32 -machine virt -m 4G  -bios ../opensbi-riscv32-generic-fw_dynamic.bin -kernel $OUTPUT_ELF # i fucking hate openbsi i will kill someone that workedn ont he development for making it start in supervisor mode instread of machine mide IN PIZDA MASII
