#include "../userlib/print.h"
#include "prog.h"
#include "../process/process.h"
#include "../timer/timer.h"
#include "../userlib/system.h"
#include "../memory/memory.h"

void prog1(void)
{
    u32 wait = 0;
    while(1)
    {
        if(wait%(2<<25) == 0)
           printf((const u8 *)"Ping from program 1\n");
        ++wait;
        yield();
    }
}

void prog2(void)
{
    u32 wait = 0;
    while(1)
    {
        if(wait%(2<<25) == 0)
            printf((const u8 *)"Ping from program 2\n");
        ++wait;
    }
}

void prog3(void)
{
    
    u32 wait = 0;
    while(1)
    {
        if(wait%(2<<25) == 0)
            printf((const u8 *)"Current CPU time(last 32 bits):%u\n",(u32)get_time());
        ++wait;
    }
}

void prog5(void)
{
    u32 CPU_time = (u32)get_time();

    u32 pseudo_random_number = (u32)(CPU_time%324535);
    pseudo_random_number^=(pseudo_random_number>>17);
    pseudo_random_number^=(pseudo_random_number>>5);

    printf((const u8 *)"Pseudo random number that represents the value that will be allocated in bytes:%u\n",pseudo_random_number);

    u32 unused_heap_mem = get_free_heap();
    printf((const u8 *)"Unused heap mem before first allocation:%u\n",unused_heap_mem);

    u8 *random_pointer = (u8 *)malloc(pseudo_random_number);

    unused_heap_mem = get_free_heap();

    printf((const u8 *)"Unused heap mem after first allocation:%u\n",unused_heap_mem);

    u32 *another_random_pointer = (u32 *)malloc(5*sizeof(u32));

    unused_heap_mem = get_free_heap();

    printf((const u8 *)"Unused heap mem after second allocation:%u\n",unused_heap_mem);

    free(random_pointer);
    unused_heap_mem = get_free_heap();

    printf((const u8 *)"Unused heap mem after free of first pointer:%u\n",unused_heap_mem);

    free(another_random_pointer);
    unused_heap_mem = get_free_heap();

    printf((const u8 *)"Unused heap mem after free of second pointer:%u\n",unused_heap_mem);

    while(1) {}
    }

void show_ticks(void)
{
    u16 wait = 0;
    prints((const u8 *)"\nShowing real time timer interrupt 'ticks'\n");
    yield();
    while(1){
        if(wait == 0)
        {
            putchar((u8)'\r');
            printf((const u8 *)"Ticks:%u",(u32)ticks);
        }
        ++wait;
    }
}