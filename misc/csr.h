#ifndef CSR_H
#define CSR_H

#include "types.h"

///mie - machine interrupt enable register; last 16 bits for normal interrupts
#define MIE_MEIE_MASK (u32)(1 << 11) // msie - machine external-level interrupt enable bit
#define MIE_MTIE_MASK (u32)(1 << 7)  // mtie - machine timer interrupt enable bit
#define MIE_MSIE_MASK (u32)(1 << 3)  // msie - machine software-level interrupt enable bit
#define MEDELEG_DISABLE_MASK (u32)(0) // disable delegation to supervisor mode 

#define MTVEC_MODE_MASK (u32)(1) // enable vectored interrupts through MTVEC reg : lsb is mode and all the other bits are the address of the ISR vector

//mip - machine interrupt pending register; last 16 bits for normal interrupts; its has machanisms to be pending

//mstatus - hart operating state register
#define MSTATUS_MIE_MASK (u32)(1 << 3) // enable mie for hart

static inline u32 read_csr_mstatus(void);
static inline u32 read_csr_mie(void); 
static inline u32 read_csr_mtvec(void); 
static inline u32 read_csr_medeleg(void); 

static inline void write_csr_mstatus(u32 mask); 
static inline void write_csr_mie(u32 mask); 
static inline void write_csr_mtvec(u32 vec_addr);
static inline void write_csr_medeleg(u32 mask);

static inline u32 read_csr_mstatus(void)
{
    u32 csr_value = 0;
    __asm__ volatile ("csrr %0, mstatus"
                        :"=r" (csr_value)
                        : // input not used
                        : // clobbered not used
                     );
    return csr_value;
}
static inline u32 read_csr_mie(void)
{
    u32 csr_value = 0;
    __asm__ volatile ("csrr %0, mstatus"
                        :"=r" (csr_value)
                        : // input not used
                        : // clobbered not used
                     );
    return csr_value;
}
static inline u32 read_csr_mtvec(void)
{
    u32 csr_value = 0;
    __asm__ volatile ("csrr %0, mstatus"
                        :"=r" (csr_value)
                        : // input not used
                        : // clobbered not used
                     );
    return csr_value;
}
static inline u32 read_csr_medeleg(void)
{
    u32 csr_value = 0;
    __asm__ volatile ("csrr %0, mstatus"
                        :"=r" (csr_value)
                        : // input not used
                        : // clobbered not used
                     );
    return csr_value;
}

static inline void write_csr_mie(u32 mask)
{                  
    __asm__ volatile ("csrw mie,%0"                 
                        : /* output not used */     
                        :"r" (mask)                 
                        : /* clobered not used */
                     );
    return;
}

static inline void write_csr_mstatus(u32 mask)
{             
    __asm__ volatile ("csrw mstatus,%0"              
                        : /* output not used */     
                        :"r" (mask)                 
                        : /* clobered not used */
                     );
    return;
}

static inline void write_csr_mtvec(u32 vec_addr)
{
    __asm__ volatile ("csrw mtvec,%0"
                        : /* output not used */ 
                        :"r" (vec_addr)
                        : /* clobered not used */
                     );
    return;
}

static inline void write_csr_medeleg(u32 mask)
{
    __asm__ volatile ("csrw medeleg,%0"
                        : /* output not used */  
                        :"r" (mask)
                        : /* clobered not used */
                     );
    return;
}

#endif