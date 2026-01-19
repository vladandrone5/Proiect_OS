#ifndef MEMORY_H
#define MEMORY_H

#include "../misc/types.h"

#define HEAP_HEADER_SIZE sizeof(heap_header)
#define MEM_ADDR_OOH (u8)-1 // mem addr out of heap, trying to free a mem address that couldnt have been allocated 
#define MEM_ADDR_NAL (u8)-2 // mem addr never llocated ie never created through malloc

typedef struct heap_header_t
{
    struct heap_header_t *next_block;
    u32 size;
    u8 is_free;
}heap_header;

void init_heap(void);
void memcpy(void *destination,void *source, u32 len);
void memset(void *destination, u32 len, u32 value);
void *malloc(u32 size);
void *calloc(u32 size,u8 value);
u8 free(void *mem);
u32 get_free_heap(void);

extern u8 HEAP_START[];
extern u8 HEAP_END[];

extern u32 HEAP_SIZE;
extern heap_header *heap;

#endif