#include "string.h"

u32 strncpy(u8 *destination, const u8 *source, u32 buff_len)
{
    u32 chars_copied = 0;

    destination[0] = '\0';

    for(u32 buff_idx = 0; buff_idx < buff_len; ++buff_idx)
    {
        if(source[buff_idx])
        {
            destination[buff_idx] = source[buff_idx];
            ++chars_copied;
        }
        else 
        {
            break;
        }
    }

    //destination[buff_len] = '\0';

    return chars_copied;
}