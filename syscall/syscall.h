#ifndef SYSCALL_H
#define SYSCALL_H

#include "../misc/types.h"

#define SYS_WRITE (u8)1
#define SYS_YIELD (u8)2
#define SYS_GETTIME  (u8)3

typedef struct{
    u32 a0,a1,a2,a3,a4,a5,a6,a7;
}syscall_frame;

extern syscall_frame syscall_args;
extern u64 syscall_return_value;

void syscall(u32 a0, u32 a1, u32 a2, u32 a3, u32 a4, u32 a5, u32 a6, u32 syscall_code);
void sys_yield(void);
void sys_write(u8 fd, u8 *buffer, u32 len);
u64  sys_get_time(void);

#endif