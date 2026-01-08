#ifndef STRING_H
#define STRING_H

#include "../misc/types.h"

// functie strlen, functie next_char_is, functie is_command, functie center

u8 numerical_to_data(u8 num_char);
u32 strncpy(u8 *destination,const u8 *source, const u32 buff_len);
u8 strncmp(const u8 *source1, const u8 *source2, const u32 buff_len);

#endif