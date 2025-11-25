#ifndef INTERRUPTS_H
#define INTERRUPTS_H

void sti_handler(void) __attribute__ ((interrupt("machine")));

void reset_interrupts(void);
void interrupts_init(void);

void stvec_idt(void)  __attribute__ ((naked, section(".text.stvec_idt") ,aligned(16)));

#endif
