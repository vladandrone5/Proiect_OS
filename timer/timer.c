#include "timer.h"
#include "../uart/uart.h"
#include "../misc/csr.h"
#include "../misc/sbi.h"

// volatile u32 *MTIME_ADDR = (u32 *)(CLINT_ADDR + MTIME_OFFSET);
// volatile u32 *MTIMECMP_ADDR = (u32 *)(CLINT_ADDR + MTIMECMP_OFFSET);

u64 timer = 0;
tint_event_counter ticks = 0;

void get_mtime(void)
{
    // CODE LEFT FOR LEARNING AND SHOWCASE PURPOSES
    // volatile u32 *mtime_high = MTIME_ADDR;
    // volatile u32 *mtime_low  = MTIME_ADDR+ 4;

    // u32 mtime_high_val = *mtime_high;
    // uart_printf((const u8 *)"HTime:%u\n",(u32)mtime_high_val);
    // u32 mtime_low_val  = *mtime_low;
    // uart_printf((const u8 *)"LTime:%u\n",(u32)mtime_low_val);

    timer = read_csr_mtime(); 
}

void set_mtimecmp(u32 os_freq)
{
// CODE LEFT FOR LEARNING AND SHOWCASE PURPOSES
//     volatile u32 *mtimecmp_high = MTIMECMP_ADDR;
//     volatile u32 *mtimecmp_low  = MTIMECMP_ADDR + 4;

//     get_mtime();

//     u64 new_mtimecmp_value = timer + (u64)(CPU_CLK_FREAKUENCY/os_freq);

//     *mtimecmp_high = (u32)(new_mtimecmp_value >> 32);
//     *mtimecmp_low  = (u32)((new_mtimecmp_value << 32) >> 32);

    sbi_set_timer(os_freq);
}

void print_ticks(void)
{
    uart_printf((const u8 *)"\nTimer interrupt(ticks:%u)...\n",(u32)ticks);
}