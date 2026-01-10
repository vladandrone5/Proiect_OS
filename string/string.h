#ifndef STRING_H
#define STRING_H

#include "../misc/types.h"

#define STRING_LIMIT (u32)(2<<16)
#define STRING_END_TIMEOUT (i8)(-1)

i32 strlen(const u8 * source);
u8 next_char_is(const u8 *source, u8 char_to_cmp);
u8 is_command(const u8 *source, const u8 *command);
u8 numerical_to_data(u8 num_char); 
u32 strncpy(u8 *destination,const u8 *source, const u32 buff_len);
u8 strncmp(const u8 *source1, const u8 *source2, const u32 buff_len);

#endif