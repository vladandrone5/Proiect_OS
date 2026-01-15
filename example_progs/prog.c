#include "../uart/print.h"
#include "prog.h"
#include "../process/process.h"
#include "../timer/timer.h"
#include "../syscall/syscall.h"

void prog1(void)
{
    u32 wait = 0;
    while(1)
    {
        if(wait%(2<<25) == 0)
           printf((const u8 *)"Ping from program 1\n");
        ++wait;
        sys_yield();
    }
}

void prog2(void)
{
    u32 wait = 0;
    while(1)
    {
        if(wait%(2<<25) == 0)
            printf((const u8 *)"Ping from program 2\n");
        ++wait;
    }
}

void show_ticks(void)
{
    u16 wait = 0;
    prints((const u8 *)"\nShowing real time timer interrupt 'ticks'\n");
    sys_yield();
    while(1){
        if(wait == 0)
        {
            putchar((u8)'\r');
            printf((const u8 *)"Ticks:%u",(u32)ticks);
        }
        ++wait;
    }
}