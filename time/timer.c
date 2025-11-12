#include "timer.h"
#include "..\uart\uart.h"

void get_mtime(void)
{
    volatile u32 *mtime_high = MTIME_ADDR;
    volatile u32 *mtime_low  = MTIME_ADDR+ 4;

    timer = (u64)(*mtime_high << 32 | *mtime_low); 
}



void set_mtimecmp(u32 cpu_clk_freq,u32 os_freq)
{
    volatile u32 *mtimecmp_high = MTIMECMP_ADDR;
    volatile u32 *mtimecmp_low  = MTIMECMP_ADDR + 4;

    get_mtime();

    u64 new_mtimecmp_value = timer + (u64)(cpu_clk_freq/os_freq);

    *mtimecmp_high = (u32)(new_mtimecmp_value >> 32);
    *mtimecmp_low  = (u32)((new_mtimecmp_value << 32) >> 32);
}

void print_timer(void)
{
    
}