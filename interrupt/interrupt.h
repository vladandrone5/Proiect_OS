#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include "../misc/types.h"

///mie - machine interrupt enable register; last 16 bits for normal interrupts
#define MIE_MSIE_MASK (u32)(1 << 3)  // msie - machine software-level interrupt enable bit
#define MIE_MTIE_MASK (u32)(1 << 7)  // mtie - machine timer interrupt enable bit

//mstatus - hart operating state register
#define MSTATUS_MIE_MASK (u32)(1 << 3) // enable mie for hart

inline void set_csr_mie(u32 mask);     
inline void set_csr_mstatus(u32 mask);  
void interrupts_init(void);

#endif
