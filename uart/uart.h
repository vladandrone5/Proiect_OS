#ifndef UART_H
#define UART_H

#include "../misc/types.h"

#define UART_ADDR (u32)0x10000000
#define NUMERIC_CHAR_OFFSET '0'

typedef union{
    const u8 *string;
    u64 uvalue;
    i64 ivalue;
}vargs_union;

extern volatile u8 *UART_WR_ADDR;
extern u8 clear_screen_sequence[4];

void uart_putchar(u8 c);
void uart_prints(const u8 *string);
void uart_printf(const u8 *format, ...);
void clear_screen(void);
//void print(const char *sep,const char *end, ...);

#endif