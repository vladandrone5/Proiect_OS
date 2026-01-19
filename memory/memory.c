#include "memory.h"
#include "../uart/uart.h"

heap_header *heap = 0;
u32 HEAP_SIZE = 0;

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

void init_heap(void)
{
    HEAP_SIZE = HEAP_END-HEAP_START;
    heap = (heap_header *)HEAP_START;
    heap->size = HEAP_SIZE - HEAP_HEADER_SIZE;
    heap->is_free = 1;
    heap->next_block = (heap_header *)0;
}

void *malloc(u32 size)
{
    // curr block is free and next was not made
    // curr block is free and next was made and is not free

    if(size>=HEAP_SIZE)
    {
        return (void *)0;
    }

    if((size%4)!=0)
    {
        size+=4-(size%4);
    }

    heap_header *curr_heap_block = heap;

    while(curr_heap_block)
    {
        if((curr_heap_block->size>=(size+HEAP_HEADER_SIZE)) && curr_heap_block->is_free)
        {

            if(curr_heap_block->next_block != 0) // in case this block was freed after being used and the one next to it isnt free
            {
            
                if((u8 *)curr_heap_block->next_block >= ((u8 *)curr_heap_block + 2*HEAP_HEADER_SIZE + size + 4)) // in case the block has to be split and the second part has at least one xlen bits alligned mem space 
                {
                    heap_header *new_heap_block = (heap_header *)((u8 *)curr_heap_block + HEAP_HEADER_SIZE + size);

                    new_heap_block->is_free = 1;
                    new_heap_block->size = curr_heap_block->size - size - HEAP_HEADER_SIZE;
                    new_heap_block->next_block = curr_heap_block->next_block;

                    curr_heap_block->next_block = new_heap_block;
                }
            }
            else
            {
                curr_heap_block->next_block = (heap_header *)((u8 *)curr_heap_block + HEAP_HEADER_SIZE + size);
                curr_heap_block->next_block->size = curr_heap_block->size-size-HEAP_HEADER_SIZE;
                curr_heap_block->next_block->is_free = 1;
                curr_heap_block->next_block->next_block = (heap_header *)0;
            }

            curr_heap_block->is_free = 0;
            curr_heap_block->size = size;

            return (void *)((u8 *)curr_heap_block+HEAP_HEADER_SIZE);
        }

        curr_heap_block = curr_heap_block->next_block;
    }

    return (void *)0;
}

u8 free(void *mem)
{
    u8 mem_allocated = 0;
    heap_header *heap_block_tbf = (heap_header *)((u8 *)mem - HEAP_HEADER_SIZE); // heap block to be feed ie mem

    if(((u8 *)heap_block_tbf)<HEAP_START || ((u8 *)heap_block_tbf)>HEAP_END)
    {
        return MEM_ADDR_OOH;
    }

    heap_header *curr_heap_block = heap;

    if((u8 *)curr_heap_block ==(u8 *)heap_block_tbf)
    {
        mem_allocated = 1;
    }
    else
    {
        while(curr_heap_block->next_block!=(heap_header *)0)
        {
            if(((u8 *)curr_heap_block->next_block == (u8 *)heap_block_tbf) && curr_heap_block->is_free)
            {
                curr_heap_block->size+= heap_block_tbf->size + HEAP_HEADER_SIZE;
                curr_heap_block->next_block = heap_block_tbf->next_block;

                heap_block_tbf = curr_heap_block;
                mem_allocated = 1;
                break;
            }

            curr_heap_block = curr_heap_block->next_block;
        }
    }


    heap_block_tbf->is_free = 1;

    if(heap_block_tbf->next_block->is_free)
    {
        heap_block_tbf->size += heap_block_tbf->next_block->size + HEAP_HEADER_SIZE;
        heap_block_tbf->next_block = (heap_header *)0;
    }


    return MEM_ADDR_NAL*mem_allocated;
}

void show_heap(void)
{
    heap_header *curr_heap_block = heap;

    uart_prints((const u8 *)"\n");

    while(curr_heap_block!=(heap_header *)0)
    {
        uart_printf((const u8 *)"Addr:%x\tSize:%x\tIsf:%u\tNext block addr:%x\n",(u8 *)curr_heap_block,curr_heap_block->size,curr_heap_block->is_free,(u8 *)curr_heap_block->next_block);
        curr_heap_block = curr_heap_block->next_block;
    } 

    uart_prints((const u8 *)"\n");
}

u32 get_free_heap(void)
{
    show_heap();

    u32 free_heap_mem = 0;

    heap_header *curr_heap_block = heap;

    while(curr_heap_block!=(heap_header *)0)
    {
        if(curr_heap_block->is_free)
        {
            free_heap_mem+=curr_heap_block->size;
        }
        curr_heap_block = curr_heap_block->next_block;
    }

    return free_heap_mem;
}

