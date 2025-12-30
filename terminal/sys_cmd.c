#include "sys_cmd.h"
#include "../process/process.h"
#include "../uart/uart.h"
#include "../timer/timer.h"
#include "../misc/csr.h"
#include "../example_progs/prog.h"
#include "../string/string.h"

void _exec(u8 id, u8 priority, const u8 *program)
{
    u8 return_code = add_process(id,priority,program);

    if(return_code == ERR_PBF)
    {
        uart_printf((const u8 *)"User has reached the maximum number of active processes!\n");
        return;
    }

    if(strncmp(process_context[current_process].process_name,(const u8 *)"prog1",6))
    {
        kernel_rpc = read_csr_sepc();
        write_csr_sepc((u32)&prog1);
    }
}

void _ps(void)
{
    uart_printf((const u8 *)"CPU time: %u ticks\n",(u32)ticks);

    if(active_processes==0)
        return;

    for(u8 pr=0;pr<8;pr++)
    {
        if(process_context[pr].state == PROCESS_ACTIVE)
        {
            uart_printf((const u8 *)"ID:%u\tPNAME:%s\n",process_context[pr].process_id, process_context[pr].process_name);
        }
    }
}