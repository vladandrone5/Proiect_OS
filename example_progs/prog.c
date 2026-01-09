#include "../uart/uart.h"
#include "prog.h"
#include "../process/process.h"
#include "../timer/timer.h"

void prog1(void)
{
    u32 wait = 0;
    while(1)
    {
        if(wait%(2<<25) == 0)
           uart_printf((const u8 *)"Ping from program 1\n");
        ++wait;
    }
}

void prog2(void)
{
    u32 wait = 0;
    while(1)
    {
        if(wait%(2<<25) == 0)
            uart_printf((const u8 *)"Ping from program 2\n");
        ++wait;
    }
}

void show_ticks(void)
{
    u16 wait = 0;
    uart_prints((const u8 *)"\n");
    while(1){
        if(wait==0)
        {
            uart_putchar((u8)'\r');
            uart_printf((const u8 *)"Ticks:%u",(u32)ticks);
        }
        ++wait;
    }
}