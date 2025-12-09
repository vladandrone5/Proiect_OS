#include "uart.h"
#include <stdarg.h>

volatile u8 *UART_WR_ADDR = (u8 *)UART_ADDR;
u8 clear_screen_sequence[4] = {0x1B,0x5B,0x32,0x4A};

void enable_uart_rx_interrupt(void)
{
    *(UART_WR_ADDR+UART_IER_OFFSET)=UART_RX_INT_MASK;
}

u8 uart_readchar(void)
{
    return *UART_WR_ADDR;
}

void clear_screen(void)
{
    uart_prints(clear_screen_sequence);
}

void uart_putchar(const u8 c)
{
    *UART_WR_ADDR=c;
    return;
}

void uart_prints(const u8 *string)
{
    while(*string)
    {
       uart_putchar(*string++);
    }
    uart_putchar((u8)'\r');
    return;
}

void uart_printf(const u8 *format, ...)
{
    va_list vargs;
    va_start(vargs, format);

    vargs_union value;

    while(*format)
    {

        if(*format != '%')
        {
            uart_putchar(*format);

            if(*format == '\n')
                uart_putchar((u8)'\r');

            ++format;    
            continue;
        }


        ++format;
        switch(*format)
        {
            case('d'): { value.ivalue = va_arg(vargs, i32); break; }
            case('u'): { value.uvalue = va_arg(vargs, u32); break; }
            case('s'): { value.string = va_arg(vargs,const u8 *); uart_prints(value.string); ++format; continue;}
        }

        u32 abs_value = value.uvalue;
        if((*format == 'd') && value.ivalue<0)
        {
            uart_putchar('-');
            abs_value = -value.ivalue;
        }

        u32 divisor = 1;

        while (abs_value / divisor > 9)
            divisor *= 10;

        while (divisor > 0) {
            uart_putchar((u8)'0' + abs_value / divisor);
            abs_value %= divisor;
            divisor /= 10;
        }

        ++format;
    }

}


// TO BE IMPLEMENTED
// void print(const char *sep,const char *end, ...)
// {
    
// }
