#include "syscall.h"
#include "../process/process.h"
#include "../misc/csr.h"
#include "../uart/uart.h"

syscall_frame syscall_args = {0};
u64 syscall_return_value = 0;

void syscall(u32 a0, u32 a1, u32 a2, u32 a3, u32 a4, u32 a5, u32 a6, u32 syscall_code)
{
    syscall_args.a0 = a0;
    syscall_args.a1 = a1;
    syscall_args.a2 = a2;
    syscall_args.a3 = a3;
    syscall_args.a4 = a4;
    syscall_args.a5 = a5;
    syscall_args.a6 = a6;
    syscall_args.a7 = syscall_code;

    __asm__ volatile("ecall");
}

void sys_yield(void)
{
    process_runtime = 255;
}

void sys_write(u8 fd, u8 *buffer, u32 len)
{
    volatile u8 *write_addr = 0;
    if(fd == 1) // write to terminal/UART
    {
        write_addr = UART_WR_ADDR;
        *write_addr = *buffer;
        return;
    }

    (void)len;
    // for(u32 buff_idx = 0; buff_idx<len; ++buff_idx)
    // {
    //     write_addr[buff_idx] = buffer[buff_idx];
    // }
}

u64 sys_get_time(void)
{
    return read_csr_mtime();
}