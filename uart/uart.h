#ifndef UART_H
#define UART_H

#include "../misc/types.h"

#define NUMERIC_CHAR_OFFSET '0'

typedef union{
    const u8 *string;
    u64 uvalue;
    i64 ivalue;
}vargs_union;

extern volatile u8 *UART_ADDR ;
extern u8 clear_screen_sequence[4];

void uart_putchar(u8 c);
void uart_prints(const u8 *string);
void uart_printf(const u8 *format, ...);
void clear_screen(void);
//void print(const char *sep,const char *end, ...);

#endif