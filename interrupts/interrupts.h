#ifndef INTERRUPTS_H
#define INTERRUPTS_H

void mti_handler(void) __attribute__ ((interrupt("machine")));
void interrupts_init(void);
void mtvec_idt(void)  __attribute__ ((naked, section(".text.mtvec_idt") ,aligned(16)));

#endif
