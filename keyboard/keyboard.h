#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "../misc/types.h"

#define KEYBOARD_SRC_ID (u32)10 /*id is 10, the same as the interrupt made by the uart; in this case with opensbi the uart is used for a keyboard driver, working as a mos; on tother systems a real plug-in keyboard should be implemented*/
#define KEYBOARD_INT_PRY (u32)7 /*highest priority on the plic, doestn matter that much as long as it is higher than the treshold and higher than others interrupts*/

#define DEBUG 0

extern u8 keys_pressed[6];
extern u8 keys_pressed_cnt;

void setup_keyboard_interrupt(u16 source, u32 priority,u32 ctx_addr);

void process_keys(u8 *keys_pressed,u8 keys_pressed_cnt);

void subroutine_ctrl_p(void);
void subroutine_enter(void);
void subroutine_backspace(void);

void subroutine_non_special_key(u8 character);

#endif