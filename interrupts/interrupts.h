#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include "../misc/types.h"

void sti_handler(void) __attribute__ ((interrupt("supervisor")));

void setup_timer_int_csrs(u32 os_freq);
void reset_interrupts(void);
void interrupts_init(void);

void stvec_idt(void)  __attribute__ ((naked, section(".text.stvec_idt") ,aligned(16)));

#endif
