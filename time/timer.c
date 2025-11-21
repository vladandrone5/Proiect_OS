#include "timer.h"
#include "../uart/uart.h"

void get_mtime(void)
{
    volatile u32 *mtime_high = MTIME_ADDR;
    volatile u32 *mtime_low  = MTIME_ADDR+ 4;

    u32 mtime_high_val = *mtime_high;
    u32 mtime_low_val  = *mtime_low;

    timer = ((u64)*mtime_high_val << 32 | *mtime_low_val); 
}

void set_mtimecmp(u32 os_freq)
{
    volatile u32 *mtimecmp_high = MTIMECMP_ADDR;
    volatile u32 *mtimecmp_low  = MTIMECMP_ADDR + 4;

    get_mtime();

    u64 new_mtimecmp_value = timer + (u64)(CPU_CLK_FREAKUENCY/os_freq);

    *mtimecmp_high = (u32)(new_mtimecmp_value >> 32);
    *mtimecmp_low  = (u32)((new_mtimecmp_value << 32) >> 32);
}

void print_timer(void)
{
    
}