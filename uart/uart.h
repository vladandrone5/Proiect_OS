#ifndef UART_H
#define UART_H

#include "../misc/types.h"
#include <stdarg.h>

#define NUMERIC_CHAR_OFFSET '0'

typedef union{
    const u8 *string;
    u32 uvalue;
    i32 ivalue;
}vargs_union;

volatile u8 *UART_ADDR = (u8 *)0x10000000;
u8 clear_screen_sequence[5] = {0x1B,0x5B,0x32,0x4A,0x00};

void uart_putchar(u8 c);
void uart_prints(const u8 *string);
void uart_printf(const u8 *format, ...);
void clear_screen(void);
void print(const char *sep,const char *end, ...);

#endif