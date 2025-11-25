#include "interrupts.h"
#include "../timer/timer.h"
#include "../misc/csr.h"
#include "../sbi/sbi.h"

#pragma GCC push_options
// Force the alignment for mtvec.BASE. A 'C' extension program could be aligned to to bytes.
#pragma GCC optimize ("align-functions=4")

void sti_handler(void)
{
    ++ticks;
    //set_mtimecmp(FREAKUENCY);
    get_mtime();
    sbi_set_timer(FREAKUENCY);
    print_timer();

}

#pragma GCC pop_options

void stvec_idt(void)
{
    __asm__ volatile (
        ".org  stvec_idt + 5*4;"
        "jal   zero,sti_handler;" 
        :
        :
        : 
        );
}

void reset_interrupts(void)
{
    clear_bit_csr_sstatus(SSTATUS_SIE_MASK);
    write_csr_sie(MIE_DISABLE_MASK);
    write_csr_satp(SATP_DISABLE_MASK);
}

void interrupts_init(void)
{
    write_csr_stvec((u32)stvec_idt | STVEC_MODE_MASK); //it is recommended to have interrupts diabled before changing mtvec
    write_csr_sie(MIE_STIE_MASK);
    write_csr_sstatus(read_csr_sstatus() | SSTATUS_SIE_MASK);

}

