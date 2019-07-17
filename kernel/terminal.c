#include <stddef.h>
#include <stdint.h>
#include "terminal.h"

static char *vram = (char*)0xB8000;
static uint8_t ter_col = 0;
static uint8_t ter_row = 0;

void ter_putchar(char ch, char color)
{
    uint16_t offset = ter_row*80*2 + ter_col*2;
    vram[offset + 0] = ch;
    vram[offset + 1] = color;

    ter_col++;
    if(ter_col == 80)
    {
        ter_col = 0;
        ++ter_row;
    }
}

void ter_setcur(uint8_t row, uint8_t col)
{
    ter_col = col;
    ter_row = row;
}


void ter_show(const char* msg)
{
    while(*msg)
    {
        ter_putchar(*msg, 0x07);
        ++msg;
    }
}
