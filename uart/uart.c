#include "uart.h"

void clear_screen(void)
{
    uart_prints(clear_screen_sequence);
}

void uart_putchar(const u8 c)
{
    *UART_ADDR=c;
    return;
}

void uart_prints(const u8 *string)
{
    while(*string)
    {
       uart_putchar(*string++);
    }
    return;
}

u32 invert_number(u32 number)
{
    u32 inverted_number=0;
    while(number)
    {
        inverted_number*=10;
        inverted_number+=number%10;
        number/=10;
    }
    return inverted_number;
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
            continue;
        }

        ++format;
        switch(*format)
        {
            case('d'): { value.ivalue = va_arg(vargs, i32); break; }
            case('u'): { value.uvalue = va_arg(vargs, u32); break; }
            case('s'): { value.string = va_arg(vargs,const u8 *); uart_prints(value.string); continue;}
        }

        u32 abs_value = 0;
        if((*format == 'd') && value.ivalue<0)
        {
            uart_putchar('-');
            abs_value = -value.ivalue;
        }

        abs_value = invert_number(abs_value);

        while(abs_value)
        {
            uart_putchar(NUMERIC_CHAR_OFFSET + abs_value%10);
            abs_value/=10;
        }
    }

}



void print(const char *sep,const char *end, ...)
{
    
}
