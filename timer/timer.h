#ifndef CLINT_TIME_H
#define CLINT_TIME_H

#include "../misc/types.h"

// #define CLINT_ADDR 0x2000000
// #define MTIME_OFFSET 0XBFF8 // csr mtime register offset in the clint
// #define MTIMECMP_OFFSET 0X4000 // csr mtimecmp register offset in the clint, used to generate timer interrupts

typedef long long unsigned int interrupt_event_counter;

#define CPU_CLK_FREAKUENCY (u32)10000000 // qemu riscv virtio internal clk freq
#define FREAKUENCY (u32)1

// extern volatile u32 *MTIME_ADDR;
// extern volatile u32 *MTIMECMP_ADDR;

extern u64 timer;
extern interrupt_event_counter ticks;

void get_mtime(void);
void set_mtimecmp(u32 freq);
void print_ticks(void);

#endif