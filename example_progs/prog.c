#include "../uart/uart.h"
#include "prog.h"
#include "../process/process.h"

void prog1(void)
{
    u32 wait = 0;
    while(1)
    {
        if(wait%(2<<16) == 0)
            uart_printf((const u8 *)"Ping from program 1\n");
        ++wait;
    }
}

void prog2(void)
{
    u32 wait = 0;
    while(1)
    {
        if(wait%(2<<16) == 0)
            uart_printf((const u8 *)"Ping from program 2\n");
        ++wait;
    }
}