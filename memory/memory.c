#include "memory.h"

void memcpy(void *destination,void *source, u32 len)
{ 
    u8 *pdest = (u8 *)destination;
    u8 *psource = (u8 *)source;
    for(u32 mem_idx = 0; mem_idx < len;mem_idx++)
    {
        *(pdest+mem_idx) = *(psource+mem_idx);
    }
}

void memset(void *destination, u32 len, u32 value)
{
    u8 *pdest = (u8 *)destination;
    for(u32 mem_idx = 0; mem_idx < len; mem_idx++)
    {
        *(pdest+mem_idx) = (u8)value;
    }
}