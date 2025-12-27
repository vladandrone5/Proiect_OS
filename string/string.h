#ifndef STRING_H
#define STRING_H

#include "../misc/types.h"

u32 strncpy(u8 *destination,const u8 *source, const u32 buff_len);
u8 strncmp(const u8 *source1, const u8 *source2, const u32 buff_len);

#endif