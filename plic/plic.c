#include "plic.h"

volatile u32 *plic_base_addr = (u32 *)PLIC_ADDR;

void reset_all_plic_source_pry(void)
{
    for(u16 source=0;source < 1024*4; source+=4)
    {
        *(u32 *)((u8 *)plic_base_addr+source) = 0;
    }
}

void reset_plic_source_pry(u16 source)
{
    *(u32 *)((u8 *)plic_base_addr+source*4) = 0;
}

void set_plic_source_pry(u16 source, u32 priority)
{
    /*Although it is a 32 bit register, the source priority can only be as high as 7*/
    *(u32 *)((u8 *)plic_base_addr+source*4) = priority % 8;
}

void reset_all_plic_int_en(u32 ctx_addr)
{
    /*iebg - interrupt enable bit group; a group of 32 bits which all represent the enabling of a different interrupt*/
    for(u32 iebg = 0; iebg< 1024/8; iebg+=4)
    {
        *(u32 *)((u8 *)plic_base_addr+ctx_addr+iebg) = 0;
    }
}

void set_plic_source_int_en(u32 ctx_addr, u16 source)
{
    *(u32 *)((u8 *)plic_base_addr+ctx_addr+source/32) |= INT_EN_BIT_OFFSET<<(source%32);
}

void reset_plic_pry_trsh(u32 ctx_addr)
{
    *(u32 *)((u8 *)plic_base_addr+ctx_addr) = 0;
}

void set_plic_pry_trsh(u32 ctx_addr, u32 treshold)
{
    *(u32 *)((u8 *)plic_base_addr+ctx_addr) = treshold % 8;
}

u32 read_claim_plic_int_msg(u32 ctx_addr)
{
    return *(u32 *)((u8 *)plic_base_addr+ctx_addr);
}

void send_complete_plic_int_ID(u32 ctx_addr, u32 ID)
{
    *(u32 *)((u8 *)plic_base_addr+ctx_addr) = ID;
}

