#ifndef SYS_CMD_H
#define SYS_CMD_H

#include "../misc/types.h"

void _exec(u8 id, u8 priority,const u8 *program);
void _kill(u8 id);
void _ps(void);
void _help(void);

#endif