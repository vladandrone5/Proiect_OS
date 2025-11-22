#ifndef CLINT_TIME_H
#define CLINT_TIME_H

#include "../misc/types.h"

#define CLINT_ADDR 0x20000000
#define MTIME_OFFSET 0XBFF8 // csr mtime register offset in the clint
#define MTIMECMP_OFFSET 0X4000 // csr mtimecmp register offset in the clint, used to generate timer interrupts

typedef long long unsigned int interrupt_event_counter;

#ifndef CPU_CLK_FREAKUENCY
#define CPU_CLK_FREAKUENCY (u32)3686400 // qemu riscv virtio internal clk freq
#endif

#define FREAKUENCY (u32)100

volatile u32 *MTIME_ADDR = (u32 *)(CLINT_ADDR + MTIME_OFFSET);
volatile u32 *MTIMECMP_ADDR = (u32 *)(CLINT_ADDR + MTIMECMP_OFFSET);

u64 timer=0;
interrupt_event_counter ticks = 0;

void get_mtime(void);
void set_mtimecmp(u32 freq);
void print_timer(void);

#endif