#ifndef MEMORY_H
#define MEMORY_H

#include "../misc/types.h"

void memcpy(void *destination,void *source, u32 len);
void memset(void *destination, u32 len, u32 value);

#endif