#include <stddef.h>
#include <stdint.h>
#include "terminal.h"

static char *vram = (char*)0xB8000;
static uint8_t ter_col = 0;
static uint8_t ter_row = 0;

static const uint8_t MAX_ROW = 25;
static const uint8_t MAX_COL = 80;

void ter_putchar(char ch, char color)
{
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
}

void ter_setcur(uint8_t row, uint8_t col)
{
    ter_col = col;
    ter_row = row;
}


void ter_show(const char* msg)
{
    while(*msg != '0')
    {
        ter_putchar(*msg, 0x07);
        ++msg;
    }
}

void ter_scroll()
{
    for(uint8_t row = 0; row < MAX_ROW - 1; ++row)
    {
        for(uint8_t col = 0; col < MAX_COL; ++col)
        {
            vram[row*80*2 + col*2] = vram[(row+1)*80*2 + col*2];
            vram[row*80*2 + col*2 + 1] = vram[(row+1)*80*2 + col*2 + 1];
        }
    }

    // clear bottom line;
    for(uint8_t col = 0; col < MAX_COL; ++col)
    {
        vram[(MAX_ROW - 1)*80*2 + col*2 + 0] = ' ';
        vram[(MAX_ROW - 1)*80*2 + col*2 + 1] = 0x07;
    }
}