#ifndef CSR_H
#define CSR_H

#include "types.h"

///mie - machine interrupt enable register; last 16 bits for normal interrupts
#define MIE_MSIE_MASK (u32)(1 << 11) // msie - machine external-level interrupt enable bit
#define MIE_MTIE_MASK (u32)(1 << 7)  // mtie - machine timer interrupt enable bit
#define MIE_MSIE_MASK (u32)(1 << 3)  // msie - machine software-level interrupt enable bit

//mip - machine interrupt pending register; last 16 bits for normal interrupts

//mstatus - hart operating state register
#define MSTATUS_MIE_MASK (u32)(1 << 3) // enable mie for hart

inline void set_csr_mie(u32 mask);     
inline void set_csr_mstatus(u32 mask);  
inline void set_csr_mtvec(u32 vec_addr);
inline void set_csr_medeleg(u32 mask);

static inline void set_csr_mie(u32 mask)
{                  
    __asm__ volatile ("csrw mie,%0"                 
                        : /* output not used */     
                        :"r" (mask)                 
                        : /* clobered not used */
                     );
    return;
}

static inline void set_csr_mstatus(u32 mask)
{             
    __asm__ volatile ("csrw mstatus,%0"              
                        : /* output not used */     
                        :"r" (mask)                 
                        : /* clobered not used */
                     );
    return;
}

static inline void set_csr_mtvec(u32 vec_addr)
{
    __asm__ volatile ("csrw mtvec,%0"
                        : /* output not used */ 
                        :"r" (vec_addr)
                        : /* clobered not used */
                     );
    return;
}

static inline void set_csr_medeleg(u32 mask)
{
    __asm__ volatile ("csrw medeleg,%0"
                        : /* output not used */  
                        :"r" (mask)
                        : /* clobered not used */
                     );
    return;
}

#endif