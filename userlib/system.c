#include "system.h"
#include "../syscall/syscall.h"

void yield(void)
{
    syscall(0,0,0,0,0,0,0,SYS_YIELD);
}

u64 get_time(void)
{
    syscall(0,0,0,0,0,0,0,SYS_GETTIME);
    u64 CPU_TIME = syscall_return_value;
    return CPU_TIME;
}