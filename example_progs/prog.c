#include "../uart/uart.h"
#include "prog.h"

void prog1(void)
{
    
    while(1)
    {
        uart_printf((const u8 *)"Ping from program 1\n");
    }
    return;
}

void prog2(void)
{
    
    while(1)
    {
        uart_printf((const u8 *)"Ping from program 2\n");
    }
    return;
}