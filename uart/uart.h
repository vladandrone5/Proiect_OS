#ifndef UART_H
#define UART_H

#include "../misc/types.h"

#define UART_ADDR (u32)0x10000000
#define UART_IER_OFFSET (u32)0x1 // interrupt enbale register
#define UART_RX_INT_MASK (u32)0x1
#define NUMERIC_CHAR_OFFSET '0'

typedef union{
    const u8 *string;
    u64 uvalue;
    i64 ivalue;
}vargs_union;

extern volatile u8 *UART_WR_ADDR;
extern u8 clear_screen_sequence[4];
extern u8 hex_char[17];

void enable_uart_rx_interrupt(void); // needed to get read data availabe interrrupt, basically keyboard interrupt

u8 uart_readchar(void);
void uart_putchar(u8 c);
void uart_prints(const u8 *string);
void uart_printf(const u8 *format, ...);
void clear_screen(void);
//void print(const char *sep,const char *end, ...);

#endif