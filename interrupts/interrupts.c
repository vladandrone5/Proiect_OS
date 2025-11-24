#include "interrupts.h"
#include "../timer/timer.h"
#include "../misc/csr.h"

#pragma GCC push_options
// Force the alignment for mtvec.BASE. A 'C' extension program could be aligned to to bytes.
#pragma GCC optimize ("align-functions=4")

void mti_handler(void)
{
    ++ticks;
    set_mtimecmp(FREAKUENCY);
    get_mtime();
}

#pragma GCC pop_options

void mtvec_idt(void)
{
    __asm__ volatile (
        ".org  mtvec_idt + 7*4;"
        "jal   zero,mti_handler;" 
        :
        :
        : 
        );
}

void interrupts_init(void)
{
    write_csr_mtvec((u32)mtvec_idt | MTVEC_MODE_MASK); //it is recommended to have interrupts diabled before changing mtvec
    write_csr_mie(MIE_MTIE_MASK);
    write_csr_mstatus(read_csr_mstatus() | MSTATUS_MIE_MASK);
    write_csr_medeleg(MEDELEG_DISABLE_MASK);
}

