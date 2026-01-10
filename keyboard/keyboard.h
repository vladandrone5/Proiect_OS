#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "../misc/types.h"

#define DEBUG 0

#define KEYBOARD_SRC_ID (u32)10 /*id is 10, the same as the interrupt made by the uart; in this case with opensbi the uart is used for a keyboard driver, working as a mos; on tother systems a real plug-in keyboard should be implemented*/
#define KEYBOARD_INT_PRY (u32)7 /*highest priority on the plic, doestn matter that much as long as it is higher than the treshold and higher than others interrupts*/

extern u8 typing_buffer[256];
extern u8 cmd_sent_buffer[256];
extern u8 keys_pressed_cnt;
extern u8 buffer_edit_idx; /*idx used when deleting characters from the middle of the temrinal line*/

void setup_keyboard_interrupt(u16 source, u32 priority,u32 ctx_addr);

void truncate_buffer(void);

void process_keys(u8 character);

void subroutine_left(void);
void subroutine_ctrl_p(void);
void subroutine_enter(void);
void subroutine_backspace(void);

void subroutine_non_special_key(u8 character);

#endif