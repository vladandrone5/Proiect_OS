#ifndef PLIC_H
#define PLIC_H

#include "../misc/types.h"

#define PLIC_ADDR (u32)0xC000000 // also the same starting addr of the 1024 source regs
#define CTX_1_IEB_ADDR (u32)0x002080 //context 1 - hart 0 S-mode , interrupt enable bits for source addr
#define CTX_1_PRY_TRSH_ADDR (u32)0x201000 // context 1, priority treshold reg addr
#define CTX_1_CLCO_ADDR (u32)0x201004 // context 1, claim/complete reg addr

#define INT_EN_BIT_OFFSET (u32)1 /*bit enable order is he same as bit order, right to left*/

extern volatile u32* plic_base_addr;

/* All functions will work on context 1 */
void reset_all_plic_source_pry(void); // set all plic source priority regs to 0
void reset_plic_source_pry(u16 source); // set to 0
void set_plic_source_pry(u16 source, u32 priority); // set to priority var
void reset_all_plic_int_en(u32 ctx_addr); // reset all enable bits
void set_plic_source_int_en(u32 ctx_adrr,u16 source); // set enable bit
void reset_plic_pry_trsh(u32 ctx_addr); // reset plic source priority treshold
void set_plic_pry_trsh(u32 ctx_addr,u32 treshold); // set plic source priority treshold
u32  read_claim_plic_int_msg(u32 ctx_addr); // read msg/id from claim/complete reg to get biggets priority interrupt ID
void send_complete_plic_int_ID(u32 ctx_addr,u32 id); // writen the ID of the interrupt that was just serviced so that it is marked as complete(stops pending)

#endif