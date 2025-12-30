#include "keyboard.h"
#include "../plic/plic.h"
#include "../uart/uart.h"
#include "../string/string.h"
#include "../process/process.h"
#include "../terminal/sys_cmd.h"

u8 typing_buffer[256] = {0};
u8 cmd_sent_buffer[256] = {0};
u8 keys_pressed_cnt = 0;
u8 buffer_edit_idx = 0;

void setup_keyboard_interrupt(u16 source,u32 priority, u32 ctx_addr)
{
    set_plic_source_pry(source, priority);
    set_plic_source_int_en(ctx_addr, source);
}

void truncate_buffer(void)
{
    for(u8 idx = buffer_edit_idx-1; idx < keys_pressed_cnt; ++idx)
    {
        typing_buffer[idx] = typing_buffer[idx+1];
    }
}

void subroutine_ctrl_p(void)
{
    uart_prints((const u8 *)"Ctrl+p pressed\n");
}

void subroutine_enter(void)
{
    uart_putchar((u8)'\n');
    uart_putchar((u8)'\r');

    typing_buffer[keys_pressed_cnt++] = '\0';

    strncpy(cmd_sent_buffer,typing_buffer, keys_pressed_cnt);

    keys_pressed_cnt = 0;
    buffer_edit_idx = 0;

    if(strncmp(cmd_sent_buffer,(const u8 *)"exec ",5))
    {
        uart_prints((const u8 *)"Entered exec subroutine!\n");
        _exec(++last_used_id, 10, cmd_sent_buffer+5);
        return;
    }
    else if(strncmp(cmd_sent_buffer,(const u8 *)"ps",2) && *(cmd_sent_buffer+2) == '\0')
    {
        _ps();
        return;
    }
}

void subroutine_backspace(void)
{
    uart_putchar((u8)8);
    uart_putchar((u8)' ');

    //truncate_buffer();

    uart_putchar((u8)8);
    uart_putchar((u8)'\0');

    --buffer_edit_idx;
    --keys_pressed_cnt;
}

void subroutine_left(void)
{
    --buffer_edit_idx;
}

void subroutine_non_special_key(u8 character)
{
    if(!DEBUG)
        uart_putchar(character);
    else
        uart_printf((const u8 *)"%u\n",(u32)character);
}

void process_keys(u8 character)
{

    switch (character)
    {
        case 16: { subroutine_ctrl_p(); break;}
        case 13: { subroutine_enter(); break;}
        case  8: { subroutine_backspace(); break;}
        default: 
        { 
            if(keys_pressed_cnt < 255){
                keys_pressed_cnt++;
                typing_buffer[buffer_edit_idx++] = character;
            }
            subroutine_non_special_key(character); 
            break;
        }
    }
}

