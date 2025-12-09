#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include "../misc/types.h"

void sti_handler(void) __attribute__ ((interrupt("supervisor")));
void seie_handler(void) __attribute__ ((interrupt("supervisor")));

void setup_timer_int_csrs(u32 os_freq);
void reset_system_interrupts(void);
void reset_plic_interrupts(void);
void reset_interrupts(void);
void system_interrupts_init(u32 sie_mask);
void plic_interrupts_init(void);
void interrupts_init(void);

void stvec_idt(void)  __attribute__ ((naked, section(".text.stvec_idt") ,aligned(16)));

#endif
