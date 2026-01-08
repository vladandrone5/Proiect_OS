#include "memory.h"

void memcpy(u8 *destination, u8 *source, u32 buff_len)
{ 
    for(u32 mem_idx = 0; mem_idx < buff_len;mem_idx++)
    {
        *(destination+mem_idx) = *(source+mem_idx);
    }
}