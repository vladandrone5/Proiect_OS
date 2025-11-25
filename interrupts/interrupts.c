#include "interrupts.h"
#include "../timer/timer.h"
#include "../misc/csr.h"

#pragma GCC push_options
// Force the alignment for mtvec.BASE. A 'C' extension program could be aligned to to bytes.
#pragma GCC optimize ("align-functions=4")

void sti_handler(void)
{
    ++ticks;
    set_mtimecmp(FREAKUENCY);
    get_mtime();
}

#pragma GCC pop_options

void stvec_idt(void)
{
    __asm__ volatile (
        ".org  stvec_idt + 7*4;"
        "jal   zero,sti_handler;" 
        :
        :
        : 
        );
}

void reset_interrupts(void)
{
    clear_bit_csr_sstatus(MSTATUS_SIE_MASK);
    write_csr_sie(MIE_DISABLE_MASK);
    write_csr_satp(SATP_DISABLE_MASK);
}

void interrupts_init(void)
{
    // write_csr_mtvec((u32)mtvec_idt | MTVEC_MODE_MASK); //it is recommended to have interrupts diabled before changing mtvec
    // write_csr_mie(MIE_MTIE_MASK);
    // write_csr_mstatus(read_csr_mstatus() | MSTATUS_MIE_MASK);

}

