#include "print.h"
#include "../syscall/syscall.h"
#include <stdarg.h>

u8 hex[17] = "0123456789abcdef";

void putchar(const u8 c)
{
   
    syscall(1,(u32)&c,1,0,0,0,0,SYS_WRITE);

    return;
}

void prints(const u8 *string)
{
    while(*string)
    {
       if(*string == '\n')
            putchar('\r');
       putchar(*string++);
    }
    return;
}

void printf(const u8 *format, ...)
{
    va_list vargs;
    va_start(vargs, format);

    vargs_union value;

    while(*format)
    {

        if(*format != '%')
        {
            putchar(*format);

            if(*format == '\n')
                putchar((u8)'\r');

            ++format;    
            continue;
        }


        ++format;
        switch(*format)
        {
            case('d'): { value.ivalue = va_arg(vargs, i32); break; }

            case('x'):
            case('u'): { value.uvalue = va_arg(vargs, u32); break; }

            case('c'): { value.uvalue = va_arg(vargs, u32); putchar((u8)value.uvalue); ++format; continue;}

            case('s'): { value.string = va_arg(vargs,const u8 *); prints(value.string); ++format; continue;}
        }

        u32 abs_value = value.uvalue;
        if((*format == 'd') && value.ivalue<0)
        {
            putchar('-');
            abs_value = -value.ivalue;
        }

        if((*format != 'x'))
        {
            u32 divisor = 1;

            while (abs_value / divisor > 9)
                divisor *= 10;

            while (divisor > 0) {
                putchar((u8)'0' + abs_value / divisor);
                abs_value %= divisor;
                divisor /= 10;
            }
        }
        else
        {
            putchar((u8)'0');
            putchar((u8)'x');

            u32 nibble_mask = 15;

            for(u8 nibblemg = 8; nibblemg > 0; --nibblemg) // nibble msb group, msb stands for the fact that it starts from left to right
            {
                putchar(hex[abs_value>>((nibblemg-1)*4) & nibble_mask]);
            }
        }

        ++format;
    }

}