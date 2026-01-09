#ifndef DEBUG_H
#define DEBUG_H

#include "../misc/types.h"

#define DEBUG 0

static void __attribute__((naked)) move_sp_to_temp(void)
{
    __asm__ volatile("mv a0,sp");
}

static u32 read_a0(void)
{
    u32 a0 = 0;
       __asm__ volatile("mv %0,a0"
                    :"=r"(a0)
                    :
                    :
                    );
    return a0;
}

static u32 read_s0(void)
{
    u32 s0 = 0;
    __asm__ volatile("mv %0,s0"
                    :"=r"(s0)
                    :
                    :
                    );
    return s0;
}

#endif