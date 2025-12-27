#ifndef SYS_CMD_H
#define SYS_CMD_H

#include "types.h"

void _exec(const u8 *program, u8 id, const u8 priority);
void _kill(u8 id);
void _ps(void);
void _help(void);

#endif