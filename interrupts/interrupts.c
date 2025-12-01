#include "interrupts.h"
#include "../timer/timer.h"
#include "../misc/csr.h"
#include "../uart/uart.h"

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

void setup_timer_int_csrs(u32 os_freq)
{
    get_mtime();
    set_mtimecmp(os_freq);
}

void reset_interrupts(void)
{
    clear_bit_csr_sstatus(SSTATUS_SIE_MASK); // disable all interrupts
    write_csr_sie(SIE_DISABLE_MASK); // clear all int enable bits
    write_csr_satp(SATP_DISABLE_MASK); // clear virtual paging reg(required for supervisor mode)?
}

void interrupts_init(void)
{
    setup_timer_int_csrs(FREAKUENCY);
    write_csr_stvec((u32)stvec_idt | STVEC_MODE_MASK); //it is recommended to have interrupts diabled before changing mtvec
    write_csr_sie(SIE_STIE_MASK);
    write_csr_sstatus(read_csr_sstatus() | SSTATUS_SIE_MASK);
}

#pragma GCC push_options
// Force the alignment for mtvec.BASE. A 'C' extension program could be aligned to to bytes.
#pragma GCC optimize ("align-functions=4")

void sti_handler(void)
{
    ++ticks;
    setup_timer_int_csrs(FREAKUENCY);
    print_ticks();
}

#pragma GCC pop_options


