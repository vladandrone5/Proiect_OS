#include "sys_cmd.h"
#include "../process/process.h"
#include "../uart/uart.h"

void _exec(const u8 *program, u8 id, const u8 priority)
{
    add_process(id,priority,program);
}

void _ps(void)
{
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