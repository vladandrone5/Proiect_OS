#include "string.h"

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
