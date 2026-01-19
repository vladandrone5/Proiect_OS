#include "sys_cmd.h"
#include "../process/process.h"
#include "../uart/uart.h"
#include "../timer/timer.h"
#include "../misc/csr.h"
#include "../example_progs/prog.h"
#include "../string/string.h"
#include "../memory/memory.h"

void _exec(u8 id, u8 priority, const u8 *program)
{
    // refactor function so that the ifs only set a variable that holds the program location and then if the last else if not ran do the return code sequence
    // make an array holding all the predetermined and global programs and search in it using the program name and the buffer
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
    else if(strncmp(program,(const u8 *)"prog3",6))
    {
        u8 return_code = add_process(id,priority,program,(u32)&prog3);

        if(return_code == ERR_PBF)
        {
            uart_printf((const u8 *)"USER HAS REACHED THE MAXIMUM NUMBER OF ACTIVE PROCESSES!\n");
            return;
        }
    }
    else if(strncmp(program,(const u8 *)"prog5",6))
    {
        u8 return_code = add_process(id,priority,program,(u32)&prog5);

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
            uart_printf((const u8 *)"\nUSER HAS REACHED THE MAXIMUM NUMBER OF ACTIVE PROCESSES!\n\n");
            return;
        }
    }
    else
    {
        uart_printf((const u8 *)"_exec error:program '%s' does not exist!\n",program);
        return;
    }
}

void _ps(void)
{
    uart_prints((const u8 *)"\n");
    uart_prints((const u8 *)" .______________________________________.\n");
    uart_prints((const u8 *)" |                                      |\n");
    uart_prints((const u8 *)" |     SYSTEM PROCESS MONITOR [v1.0]    |\n");
    uart_prints((const u8 *)" |______________________________________|\n");
    
    uart_printf((const u8 *)" | CPU_TICKS  : %u \n", (u32)ticks);
    uart_prints((const u8 *)" |______________________________________|\n");
    uart_prints((const u8 *)"\n");

    if(active_processes == 0)
    {
        uart_prints((const u8 *)"   [!] NO ACTIVE TASKS DETECTED\n\n");
        return;
    }

    uart_prints((const u8 *)"   ID  |  PROCESS_NAME    |    STATUS     \n");
    uart_prints((const u8 *)"  ------------------------------------    \n");

    for(u8 pr = 0; pr < 8; pr++)
    {
        if(process_list[pr] == 0)
            break;

        if(process_list[pr]->state != PROCESS_DEAD && process_list[pr]->state != PROCESS_INACTIVE)
        {
            uart_printf((const u8 *)"   #%u  | %s\t          |  [RUNNING]\n", 
                process_list[pr]->process_id, 
                process_list[pr]->process_name
            );
        }
    }
    
    uart_prints((const u8 *)"\n   > END OF LIST\n\n");

}

void _kill(u8 id)
{
    if(active_processes == 0)
    {
        uart_prints((const u8 *)"_kill error:No programs currently running...\n");
        return;
    }

    for(u8 pr = 0; pr<8; pr++)
    {
        if(process_list[pr] == 0)
        {
            break;
        }

        if(process_list[pr]->process_id == id)
        {
            remove_process(pr);
            return;
        }
    }

    uart_printf((const u8 *)"_kill_error:No current program running on ID:%u!\n",(u32)id);
}

void _shell(void)
{
    uart_prints((const u8 *)"<EdgarieOS>:");
}

void _clear(void)
{
    clear_screen();
}

void _free_heap_online_click_download(void)
{
    static u8 uses = 0;

    if(uses == 1)
    {
        uart_prints((const u8 *)" Why did they lock him up?\n");
    }

   u32 free_heap_mem = get_free_heap();

    uart_printf((const u8 *)"\nCurrent unused heap memory:%u MB (%u bytes)\n\n",free_heap_mem/1000000,free_heap_mem);

    if(uses == 2)
    {
        uart_prints((const u8 *)"No like serious Why did they lock him up?\n");
    }

    ++uses;
}

void _help(void)
{
    //uart_prints((const u8 *)"\nAvailable commands:\n\t'exec': executes a program given as the first command line argument(max 8 programs runnign at the same time)\n\t\tprog1: -prints message and yields\n\t\tprog2: -prints message faster\n\t\tprog3: -prints CPU time\n\t\tprog5: -heap mem allocation demo\n\t\tshow_ticks: -prints 'ticks'(number of timer interrupts\n\t'ps': shows processor state (ticks and active processes)\n\t'kill': stops process with id identical to the first command line argument given\n\t'clear' clears terminal screen\n\t'free my boy heap': prints available heap memory\nREMEBER WHO YOU ARE!!!\n\n")
    uart_prints((const u8 *)"\n");
    uart_prints((const u8 *)"  /$$   /$$ /$$$$$$$$ /$$       /$$$$$$$\n");
    uart_prints((const u8 *)" | $$  | $$| $$_____/| $$      | $$__  $$\n");
    uart_prints((const u8 *)" | $$  | $$| $$      | $$      | $$  \\ $$\n");
    uart_prints((const u8 *)" | $$$$$$$$| $$$$$   | $$      | $$$$$$$/\n");
    uart_prints((const u8 *)" | $$__  $$| $$__/   | $$      | $$____/ \n");
    uart_prints((const u8 *)" | $$  | $$| $$      | $$      | $$      \n");
    uart_prints((const u8 *)" | $$  | $$| $$$$$$$$| $$$$$$$$| $$      \n");
    uart_prints((const u8 *)" |__/  |__/|________/|________/|__/      \n");
    uart_prints((const u8 *)"\n   [ EDGARIE OS SYSTEM COMMANDS ]\n\n\n");

    uart_prints((const u8 *)"  > exec <program_name>\n");
    uart_prints((const u8 *)"      Executes a program (Max 8 concurrent).\n");
    uart_prints((const u8 *)"      Available programs:\n");
    uart_prints((const u8 *)"        - prog1      : Prints message and yields\n");
    uart_prints((const u8 *)"        - prog2      : Prints message faster\n");
    uart_prints((const u8 *)"        - prog3      : Prints CPU time\n");
    uart_prints((const u8 *)"        - prog5      : Heap allocation demo\n");
    uart_prints((const u8 *)"        - show_ticks : Prints timer interrupts\n\n");

    // PS Command
    uart_prints((const u8 *)"  > ps\n");
    uart_prints((const u8 *)"      Shows process state table & CPU ticks.\n\n");

    // KILL Command
    uart_prints((const u8 *)"  > kill <id>\n");
    uart_prints((const u8 *)"      Terminates process with specific ID.\n\n");

    // CLEAR Command
    uart_prints((const u8 *)"  > clear\n");
    uart_prints((const u8 *)"      Clears the terminal screen.\n\n");

    // MEMORY Command
    uart_prints((const u8 *)"  > free my boy heap\n");
    uart_prints((const u8 *)"      Prints available heap memory.\n\n");

    uart_prints((const u8 *)"  ------------------------------------------\n");
    uart_prints((const u8 *)"          !!!REMEMBER WHO YOU ARE!!!\n\n");
}

void _easter_egg(void)
{
    uart_prints((const u8 *)"=======+++*%@%%@%%%####%%@@@%%@@@@@@@%%%@@@@@@@@@@@@@%%@%%%%@%%@@@@@@@@@@@@@@@@%*+++++++++++++++++++\n");
    uart_prints((const u8 *)"========+#%%%%@@%%##%%%%%@%%@@@%%%%%%%%%%@@@%%%%%%%%%%%####%%%%%%%@@@@@@@@@@@@@@@#**++++++++++++++++\n");
    uart_prints((const u8 *)"=====++*%%%#%%@@@@@%%%%%%%%%%%%%%%%%%%%%%##%#*****+++***++****###%%%%%%%%%@@@@@@@#*+++++++++++++++++\n");
    uart_prints((const u8 *)"=====+*#%%%###%%@@@@@@@@%%%%%######***++++++==========++++++++***########%@@@@@@@@#+++++++++++++++++\n");
    uart_prints((const u8 *)"====++#%%%%@@@@@@@@@%%##****+**+++++========--------=======++++***###%%##%%%%@@@@@%*++++++++++++++++\n");
    uart_prints((const u8 *)"====*%%%%%@@@@@@@%%#**+++=================---------------====++++*###%%%##%%%%%@@@%#++++++++++++++++\n");
    uart_prints((const u8 *)"==+*#%%%%%@%%%%%%#*++=======------------------------------======++*###%%##%%%%%@@@%#++++++++++++++++\n");
    uart_prints((const u8 *)"=+**##%%@%%%%%%%#*+=======----------------------------------====+++*##%%%%%%%%%@@@@#++++++++++++++++\n");
    uart_prints((const u8 *)"==+*%%%%%@@%%%%%*++====--------------------------------------====++**####%%%%%%@@@@%++++++++++++++++\n");
    uart_prints((const u8 *)"=+*#%%%@@@@%%%%#*+====---------------------------------------====++++*###%%%%%%@@@@%++++++++++++++++\n");
    uart_prints((const u8 *)"+++*%@@@@@@@@@#*++====---------------------------------------=====++++*###%%%%@@@@@%++++++++++++++++\n");
    uart_prints((const u8 *)"++=+%@@@@@@@@%**++===-------------------::::::::-::------==----====++++*##%%%%@@@@@%*+++++++++++++++\n");
    uart_prints((const u8 *)"===*%@@@@@@@@%#*+===----------------:::::::::::::-===*##**####*+++++++++*#%%%@@@@@@@*+++++++++++++++\n");
    uart_prints((const u8 *)"===+%%@@@@@@%##*+====--------------:::::::::----*%%%%%#*****+****#*++++++#%%%@@@@@@#++++++++++++++++\n");
    uart_prints((const u8 *)"====+%@@@@@@%%#*+====--------------:::::---==+++#####+=====++++++++++++++*#%@@@@#+-=++++++++++++++++\n");
    uart_prints((const u8 *)"=====+#@@@@@@%#*+===--------------------:--=++++**+++++++++*###**+====++++*%@@@#==+###**++++++++++++\n");
    uart_prints((const u8 *)"======*%@@@@@%**+==--===+**#####*+===---::-==++*****+****#%####*+=======+++#%@@+=++=++++++++++++++++\n");
    uart_prints((const u8 *)"======+#@@@@@%*++==+###%%%####***+++=--------=+**++*+++*#%*++***==-----===+*%@%+++=--++==+++++++++++\n");
    uart_prints((const u8 *)"=======*@@@@@%*+++*#*+======++***++++=------==+===+==---===++==--------===++%@%+++=--=+==+++++++++++\n");
    uart_prints((const u8 *)"========*%@@@%*+##+===++===+****+++++=------======-===========----------=+++#@%**#*+=-===+++++++++++\n");
    uart_prints((const u8 *)"========+*%%@%*+*+==+*++*#%%%*=-====+=-------=======-------------------===++*@%+=========+++++++++++\n");
    uart_prints((const u8 *)"=========*%%@@#+++===*%%+==++=--======-----------==--------------------===++*#%+==--==-=++++++++++++\n");
    uart_prints((const u8 *)"==========+#%@#++===+**++=============-------====---------------------====+**#%#==--=-=+++++++++++++\n");
    uart_prints((const u8 *)"==========+*%@%++===-========-----====--------====--------------------====+++*%%*====+==++++++++++++\n");
    uart_prints((const u8 *)"=========+*==#%*==-----------------===--------=====-------------------====+++*%%*--====+++++++++++++\n");
    uart_prints((const u8 *)"==========**+*%#==-----------------===---::----=====----------------=====++***#%*===-=++++++++++++++\n");
    uart_prints((const u8 *)"===========+++*#+==-----------------==------=======+=---------------=====+++**#%#=-=+++++=++++++++++\n");
    uart_prints((const u8 *)"==============+**==----------------===+==-==*%%%#+++=---------------=====+++**#%%#*+++++++++++++++++\n");
    uart_prints((const u8 *)"===============*#+==---------------=+*%%**+***++**+-----------------==+=+++++*#%%@*=++++++++++++++++\n");
    uart_prints((const u8 *)"==============-*#+==----------------+++=++++=++==------------------======++++*##@%*+++++++++++++++++\n");
    uart_prints((const u8 *)"===============+%#===----------------===++======--=============----====++++***#%@%++++++++++++++++++\n");
    uart_prints((const u8 *)"===============+#%++===-------------=++===---------------==++++=--=====+++++*##%%#++++++++++++++++++\n");
    uart_prints((const u8 *)"==============++*%#++===---------=====--------------===++***+++=--====++++++*#%@%*++++++++++++++++++\n");
    uart_prints((const u8 *)"================*%%*++====-----====----=====++++++*****###*=--=---===++*+**##%%@%*++++++++++++++++++\n");
    uart_prints((const u8 *)"================+#%#+++====----=++=+++*****++=====-=======-------===+++***###%@@#+++++++++++==++++++\n");
    uart_prints((const u8 *)"=================+%%*++=====--===+*##*++=---------=+++==--------====+**##%%%%@@@*=+++++++=====++++++\n");
    uart_prints((const u8 *)"==================+%%*++=====--==--==++++++====++++===--=----====+++*#%%%%%@@@@%*=++++++======+=++++\n");
    uart_prints((const u8 *)"===================*%#*+++++==-------=======+++++=====------===+***#%%%%%@@@@@@%*+++++++========++++\n");
    uart_prints((const u8 *)"===================+#%#**++++==------========++**++=-------=++**#%%%%@@%@@@@@@%%++++++++========++++\n");
    uart_prints((const u8 *)"====================+#%%#+**+++=====-===---==++++==+=----==+**#%%%%%%@@@@@@@@%##+==+++++========++++\n");
    uart_prints((const u8 *)"==================+==*%@%%#***+=++++===-----=++++==+++=++++**##%%@@@@@@@@@@%##*#+-=+++++========++++\n");
    uart_prints((const u8 *)"=====================+%@@@%%%####%##*+==--==+++++++==+*****#%%%%@@@@@@@@@%%##***+:-+++++++++==++++++\n");
    uart_prints((const u8 *)"=======================#@@@@@%##%%%%#**+++++++++++++++**####%%%@@@@@@@@@%##****+-:--++=+++++====++++\n");
    uart_prints((const u8 *)"==================+==+==*@@@@%%%%%%%%###****+*******#**##%%%%@@@@@@@@%###***+++=::--=#*+++++====++++\n");
    uart_prints((const u8 *)"=======================++=*@@@@@@@@@@%%%###*****#*#%%%%%%%@@@@@@@@%%##****++++-.::-::=%@#+===+=+++++\n");
    uart_prints((const u8 *)"=========================++*%@@@@@@@@@@%%%%%%%%%%%%%%%@@@@@@@@@@%####***+++++:.::::-::+@@@%*+=====++\n");
    uart_prints((const u8 *)"============================**#%@@@@%@@@@@@@@@@%%%%@@@@@@@@@@@%####***+++=+=:.:::::::::#@@@@@%*+==++\n");
    uart_prints((const u8 *)"==========================+##****##%%%%%%@@@@@@@@%%@@@@@@@%####******+++++:..::::::::::-%@@@@@@@%#*=\n");
    uart_prints((const u8 *)"========================*%@@*-=**+**#####%%%%%%@@@%%%%%##*+*****+*****++-..:::::::::::::=%@@@@@@@@@@\n");
    uart_prints((const u8 *)"===================+#%@@@@@@*:::=**++++************++++==+++****++++++-...::::::::.:::::-%@@@@@@@@@@\n");
    uart_prints((const u8 *)"==========-==+*%%@@@@@@@@@@@*:::.:=++++++++++*+++++++++==++*+++*++++=:.....:::.:....::::=%@@@@@@@@@@\n");
    uart_prints((const u8 *)"==---==++#%@@@@@@@@@@@@@@@@@+-::::.:=+++++++++++++++++++++**++++++=.................:::.=@@@@@@@@@@@\n");
}