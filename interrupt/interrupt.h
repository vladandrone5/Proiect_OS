#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include "../misc/csr.h"

void mti_handler(void) __attribute__ ((interrupt("machine")));

void interrupts_init(void);

#endif
