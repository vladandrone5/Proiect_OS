#include "string.h"
#include "../uart/uart.h"

i32 strlen(const u8 *source)
{
    u32 char_cnt = 0;

    while(*source++)
    {
        ++char_cnt;

        if(char_cnt>=STRING_LIMIT)
        {
            return (i32)STRING_END_TIMEOUT;
        }
    }

    return char_cnt;
}

u8 next_char_is(const u8 *source, u8 char_to_cmp)
{
    return *(source+1) == char_to_cmp;
}

u8 numerical_to_data(u8 *num_char)
{
    // transforms ascii numerical chars into their represented value ex: '7' -> 7
    // check if its a numer as a char
    i32 len = strlen(num_char);
    u8 tens_pow = 1;
    u8 numerical_value = 0;

    for(u8 pow=0;pow<(u8)len-1;pow++)
    {
        tens_pow*=10;
    }

    for(u8 buff_idx=0;buff_idx<(u8)len;buff_idx++)
    {
        numerical_value+=tens_pow*(num_char[buff_idx]-'0');
        tens_pow/=10;
    }

    uart_printf((const u8 *)"Num value:%u\n",numerical_value);

    return numerical_value;
}

u32 strncpy(u8 *destination, const u8 *source, const u32 buff_len)
{
    u32 chars_copied = 0;

    destination[0] = '\0';

    u32 buff_idx = 0;

    for(; buff_idx <= buff_len; ++buff_idx)
    {
       
        destination[buff_idx] = source[buff_idx];
        ++chars_copied;

        if(!source[buff_idx])
        {
            break;
        }
    }

    destination[buff_idx] = '\0';

    return chars_copied;
}

u8 strncmp(const u8 *source1, const u8 *source2, const u32 buff_len)
{
    for(u32 buff_idx = 0; buff_idx < buff_len; buff_idx++)
    {
        if(source1[buff_idx]!=source2[buff_idx])
        {
            return 0;
        }
    }
    return 1;
}

u8 is_command(const u8 *source, const u8 *command)
{
    i32 command_len = strlen(command);

    if(command_len<0)
    {
        return (u8)0;
    }

    if(strncmp(source, command, command_len) && (next_char_is(source+command_len-1,' ') || next_char_is(source+command_len-1,'\0')))
    {
        return (u8)1;
    }

    return (u8)0;
}