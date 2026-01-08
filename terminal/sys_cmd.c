#include "sys_cmd.h"
#include "../process/process.h"
#include "../uart/uart.h"
#include "../timer/timer.h"
#include "../misc/csr.h"
#include "../example_progs/prog.h"
#include "../string/string.h"

void yield(void)
{
    process_runtime = process_context[current_process].time_slice;
}

void _exec(u8 id, u8 priority, const u8 *program)
{
    // refactor function so that the ifs only set a variable that holds the program location and then if the last else if not ran do the return code sequence
    if(strncmp(program,(const u8 *)"prog1",6))
    {
        u8 return_code = add_process(id,priority,program,(u32)&prog1);
        
        if(return_code == ERR_PBF)
        {
            uart_printf((const u8 *)"USER HAS REACHED THE MAXIMUM NUMBER OF ACTIVE PROCESSES!\n");
            return;
        }
    }

    else if(strncmp(program,(const u8 *)"prog2",6))
    {
        u8 return_code = add_process(id,priority,program,(u32)&prog2);

        if(return_code == ERR_PBF)
        {
            uart_printf((const u8 *)"USER HAS REACHED THE MAXIMUM NUMBER OF ACTIVE PROCESSES!\n");
            return;
        }
    }
    else if(strncmp(program,(const u8 *)"show_ticks",11))
    {
        u8 return_code = add_process(id,priority,program,(u32)&show_ticks);

        if(return_code == ERR_PBF)
        {
            uart_printf((const u8 *)"USER HAS REACHED THE MAXIMUM NUMBER OF ACTIVE PROCESSES!\n");
            return;
        }
    }
    else
    {
        uart_printf((const u8 *)"_exec error:program '%s' does not exist!\n",program);
    }
}

void _ps(void)
{
    uart_printf((const u8 *)"\n##############################\n#                            #\n#       PROCESS  STATE       #\n#                            #\n##############################\n\n");
    uart_printf((const u8 *)"| CPU TIME: <%u> ticks |\n\n",(u32)ticks);

    if(active_processes==0)
        return;

    for(u8 pr=0;pr<8;pr++)
    {
        if(process_context[pr].state != PROCESS_DEAD && process_context[pr].state != PROCESS_INACTIVE)
        {
            uart_printf((const u8 *)"> ID:%u\tPNAME:%s\n",process_context[pr].process_id, process_context[pr].process_name);
        }
    }
    uart_prints((const u8 *)"\n");
}

void _kill(u8 id)
{
    if(active_processes == 0)
        return;

    for(u8 pr = 0; pr<8; pr++)
    {
        if(process_context[current_process].process_id == id)
        {
            uart_prints((const u8 *)"Removed program\n");
            remove_process();
            return;
        }
    }
}

void _shell(void)
{
    uart_prints((const u8 *)"<EdgarieOS>:");
}

void _clear(void)
{
    clear_screen();
}