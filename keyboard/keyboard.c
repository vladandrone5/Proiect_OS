#include "keyboard.h"
#include "../plic/plic.h"
#include "../uart/uart.h"

u8 keys_pressed[6] = {0};
u8 keys_pressed_cnt = 0;

void setup_keyboard_interrupt(u16 source,u32 priority, u32 ctx_addr)
{
    set_plic_source_pry(source, priority);
    set_plic_source_int_en(ctx_addr, source);
}

void subroutine_ctrl_p(void)
{
    uart_prints((const u8 *)"Ctrl+p pressed\n");
}

void subroutine_enter(void)
{
    uart_putchar((u8)'\n');
    uart_putchar((u8)'\r');
}

void subroutine_backspace(void)
{
    uart_putchar((u8)8);
    uart_putchar((u8)' ');
    uart_putchar((u8)8);
    uart_putchar((u8)'\0');
}

void subroutine_non_special_key(u8 character)
{
    if(!DEBUG)
        uart_putchar(character);
    else
        uart_printf((const u8 *)"%u\n",(u32)character);
}

void process_keys(u8 *keys_pressed, u8 keys_pressed_cnt)
{
    for(u8 key_idx=0;key_idx<keys_pressed_cnt;key_idx++)
    {
        switch (keys_pressed[key_idx])
        {
            case 16: { subroutine_ctrl_p(); break;}
            case 13: { subroutine_enter(); break;}
            case  8: { subroutine_backspace(); break;}
            default: {subroutine_non_special_key(keys_pressed[key_idx]); break;}
        }
    }
}

