#include <stddef.h>
#include <stdint.h>
#include "terminal.h"
#include "uart.h"
#include "x86.h"

static volatile char *vram = (volatile void*)0xB8000;
static volatile uint16_t *vram_fast = (volatile void*)0xB8000;
static uint8_t ter_col = 0;
static uint8_t ter_row = 0;
static char ter_color = TER_WHITE;
static const uint8_t MAX_ROW = 25;
static const uint8_t MAX_COL = 80;



void ter_putchar(char ch)
{   
    char color = ter_color;
    if(ter_col == 80)
    {
        ter_col = 0;
        ++ter_row;
    }

    if(ter_row == 25)
    {
        ter_scroll();
        ter_col = 0;
        ter_row = MAX_ROW - 1;
    }


    if(ch == '\n')
    {
        ++ter_row;
        ter_col = 0;
        return;
    }

    uint16_t offset = ter_row*80*2 + ter_col*2;
    vram[offset + 0] = ch;
    vram[offset + 1] = color;

    ter_col++;
    ter_update_cur();

}

void ter_setcur(uint8_t row, uint8_t col)
{
    ter_col = col;
    ter_row = row;
}


void ter_show(const char* msg, const char color)
{
    char last_color = ter_col;
    ter_col = color;
    while(*msg)
    {
        ter_putchar(*msg);
        ++msg;
    }
    ter_col = last_color;
}

void ter_scroll()
{
    uint16_t offset = 0;

    while(offset < MAX_COL * (MAX_ROW - 1))
    {
        vram_fast[offset] = vram_fast[offset + MAX_COL];
        ++offset;     
    }

    // clear bottom line;
    for(uint8_t col = 0; col < MAX_COL; ++col)
    {
        vram[(MAX_ROW - 1)*80*2 + col*2 + 0] = ' ';
        vram[(MAX_ROW - 1)*80*2 + col*2 + 1] = 0x07;
    }
}

void ter_clear()
{
    uint16_t offset = 0;
    uint16_t empty_char = ' ' | (0x07 << 8);
    while (offset < MAX_COL * MAX_ROW)
    {
        vram_fast[offset++] = empty_char; 
    }
}


void ter_setcolor(char color)
{
    ter_color = color;
}

void ter_init()
{
    ter_clear();
}

void ter_update_cur()
{
    uint16_t cursor_pos = ter_row * MAX_COL + ter_col;
    x86_outb(0x3d4, 0x0f);
    x86_outb(0x3d5, (cursor_pos) & 0xff);
    x86_outb(0x3d4, 0x0e);
    x86_outb(0x3d5, (cursor_pos >> 8) & 0xff);
}

