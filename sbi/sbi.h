#ifndef SBI_H
#define SBI_H

#include "../misc/types.h"
#include "../timer/timer.h"

// SBI Extension ID for setting the timer
#define SBI_EID_TIME 0x54494D45

// SBI Function ID for setting the timer
#define SBI_FID_TIME_SET_TIMER 0

static inline void sbi_ecall(u32 ext_id, u32 func_id, u32 arg0, u32 arg1);
static void sbi_set_timer(u32 freq);

static inline void sbi_ecall(u32 ext_id, u32 func_id, u32 arg0, u32 arg1)
{
    // The current QEMU/OpenSBI implementation for RV32 uses two 32-bit registers 
    // (a0/a1) to pass the 64-bit time value, which simplifies the assembly.
    // u64 value is passed as (a0, a1) pair.
    
    register u32 a0_reg __asm__("a0") = arg0;
    register u32 a1_reg __asm__("a1") = arg1;
    register u32 a6_reg __asm__("a6") = func_id;
    register u32 a7_reg __asm__("a7") = ext_id;

    __asm__ volatile (
        "ecall"
        : "+r" (a0_reg)  // a0 is input and output
        , "+r" (a1_reg)  // a1 is input and output
        : "r" (a6_reg)
        , "r" (a7_reg)
        : "memory"
    );
    
    // In a real OS, we would check the return values in a0 (error code) and a1 (return data).
    // For this simple bootloader, we ignore the return.
}

static void sbi_set_timer(u32 freq)
{
    // In RV32, the 64-bit time_value is split into two 32-bit arguments (a0 and a1)
    u32 timer_int_offset = CPU_CLK_FREAKUENCY/freq;
    u64 new_stimecp = timer + (u64)timer_int_offset;
    u32 new_stimecmp_low = (u32)(new_stimecp & 0xFFFFFFFF);
    u32 new_stimecmp_high = (u32)(new_stimecp >> 32);

    sbi_ecall(
        SBI_EID_TIME,       // a7: Extension ID
        SBI_FID_TIME_SET_TIMER, // a6: Function ID
        new_stimecmp_low,      // a0: Argument 1 (Time Value Low)              
        new_stimecmp_high                 // a1: Argument 2 (Time Value High)
    );
}

#endif