#include "uart.h"

void clear_screen(void)
{
    uart_print(clear_screen_sequence);
}

void uart_putchar(u8 c)
{
    *UART_ADDR=c;
    return;
}

void uart_prints(const u8 *string)
{
    while(*string)
    {
       uart_putchar((u8)*string++);
    }
    return;
}

void print(char *sep,char *end, ...)
{
    
}
