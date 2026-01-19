#ifndef PRINT_H
#define PRINT_H

#include "../misc/types.h"

#define NUMERIC_CHAR_OFFSET '0'

typedef union{
    const u8 *string;
    u64 uvalue;
    i64 ivalue;
}vargs_union;

extern u8 hex[17];

void putchar(u8 c);
void prints(const u8 *string);
void printf(const u8 *format, ...);

#endif