#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "../misc/types.h"

#define KEYBOARD_SRC_ID (u32)10 /*id is 10, the same as the interrupt made by the uart; in this case with opensbi the uart is used for a keyboard driver, working as a mos; on tother systems a real plug-in keyboard should be implemented*/
#define KEYBOARD_INT_PRY (u32)7 /*highest priority on the plic, doestn matter that much as long as it is higher than the treshold and higher than others interrupts*/

void setup_keyboard_interrupt(u16 source, u32 priority,u32 ctx_addr);

#endif