#ifndef UART_H
#define UART_H

#include "../misc/types.h"
#include <stdarg.h>

volatile u8 *UART_ADDR = (u8 *)0x10000000;
u8 clear_screen_sequence[4] = {0x1B,0x5B,0x32,0x4A};

void uart_putchar(u8 c);
void uart_prints(const u8 *string);
void clear_screen(void);
void print(const char *sep,const char *end, ...);

#endif