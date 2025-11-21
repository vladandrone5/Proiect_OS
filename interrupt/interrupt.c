#include "interrupt.h"
#include "../time/timer.h"

void mti_handler(void)
{
    set_mtimecmp(FREAKUENCY);
    get_mtime();
}

void interrupts_init(void)
{
    set_csr_mie(MIE_MSIE_MASK);
    set_csr_mie(MIE_MTIE_MASK);
    set_csr_mstatus(MSTATUS_MIE_MASK);
}