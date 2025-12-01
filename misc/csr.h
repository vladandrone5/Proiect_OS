#ifndef CSR_H
#define CSR_H

#include "types.h"

///mie - machine interrupt enable register; last 16 bits for normal interrupts
#define SIE_SEIE_MASK (u32)(1 << 9) // msie - machine external-level interrupt enable bit
#define SIE_STIE_MASK (u32)(1 << 5)  // mtie - machine timer interrupt enable bit
#define SIE_SSIE_MASK (u32)(1 << 1)  // msie - machine software-level interrupt enable bit
#define SIE_DISABLE_MASK (u32)(0)
#define MEDELEG_DISABLE_MASK (u32)(0) // disable delegation to supervisor mode 
#define SATP_DISABLE_MASK (u32)(0)

#define STVEC_MODE_MASK (u32)(1) // enable vectored interrupts through MTVEC reg : lsb is mode and all the other bits are the address of the ISR vector

//mip - machine interrupt pending register; last 16 bits for normal interrupts; its has machanisms to be pending

//mstatus - hart operating state register
#define SSTATUS_SIE_MASK (u32)(1 << 1) // enable mie for hart

static inline u32 read_csr_sstatus(void);
static inline u32 read_csr_sie(void); 
static inline u32 read_csr_stvec(void); 
static inline u32 read_csr_satp(void);
static inline u64 read_csr_mtime(void);

static inline void clear_bit_csr_sstatus(u32 mask);

static inline void write_csr_sstatus(u32 mask); 
static inline void write_csr_sie(u32 mask); 
static inline void write_csr_stvec(u32 vec_addr);
static inline void write_csr_satp(u32 mask);

static inline u64 read_csr_mtime(void)
{
    u32 timel=0,timeh=0;

    __asm__ volatile("csrr %0, time"
                    :"=r" (timel)
                    :
                    :
                    );

    __asm__ volatile("csrr %0,timeh"
                    :"=r" (timeh)
                    :
                    :
                    );
    
    return (((u64)timeh << 32) | timel);
}

static inline u32 read_csr_satp(void)
{
    u32 csr_value=0;
    __asm__ volatile ("csrr %0,satp"
                        :"=r" (csr_value)
                        :
                        :
                     );
    return csr_value;
}

static inline void write_csr_satp(u32 mask)
{
    __asm__ volatile("csrw satp,%0"
                        :
                        :"r" (mask)
                        :
                     );
    return;
}

static inline void clear_bit_csr_sstatus(u32 mask)
{
    __asm__ volatile ("csrc sstatus, %0"
                        :
                        :"r" (mask)
                        :
                     );
    return;
}

static inline u32 read_csr_sstatus(void)
{
    u32 csr_value = 0;
    __asm__ volatile ("csrr %0, sstatus"
                        :"=r" (csr_value)
                        : // input not used
                        : // clobbered not used
                     );
    return csr_value;
}
static inline u32 read_csr_sie(void)
{
    u32 csr_value = 0;
    __asm__ volatile ("csrr %0, sie"
                        :"=r" (csr_value)
                        : // input not used
                        : // clobbered not used
                     );
    return csr_value;
}
static inline u32 read_csr_stvec(void)
{
    u32 csr_value = 0;
    __asm__ volatile ("csrr %0, stvec"
                        :"=r" (csr_value)
                        : // input not used
                        : // clobbered not used
                     );
    return csr_value;
}

static inline void write_csr_sie(u32 mask)
{                  
    __asm__ volatile ("csrw sie,%0"                 
                        : /* output not used */     
                        :"r" (mask)                 
                        : /* clobered not used */
                     );
    return;
}

static inline void write_csr_sstatus(u32 mask)
{             
    __asm__ volatile ("csrw sstatus,%0"              
                        : /* output not used */     
                        :"r" (mask)                 
                        : /* clobered not used */
                     );
    return;
}

static inline void write_csr_stvec(u32 vec_addr)
{
    __asm__ volatile ("csrw stvec,%0"
                        : /* output not used */ 
                        :"r" (vec_addr)
                        : /* clobered not used */
                     );
    return;
}

#endif