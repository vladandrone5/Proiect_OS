#include "interrupt.h"

inline void set_csr_mie(u32 mask)
{                  
    __asm__ volatile ("csrw mie,%0"                 
                        : /* output not used */     
                        :"r" (mask)                 
                        : /* clobered not used */
                     );
    return;
}

inline void set_csr_mstatus(u32 mask)
{             
    __asm__ volatile ("csrw mstatus,%0"              
                        : /* output not used */     
                        :"r" (mask)                 
                        : /* clobered not used */
                     );
    return;
}

void interrupts_init(void)
{
    set_csr_mie(MIE_MSIE_MASK);
    set_csr_mie(MIE_MTIE_MASK);
    set_csr_mstatus(MSTATUS_MIE_MASK);
}