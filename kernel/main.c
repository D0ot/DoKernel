#include <stddef.h>
#include <stdint.h>
#include "terminal.h"
#include "x86.h"


uint8_t *buf = (uint8_t*)0x10000;

void test_inoutb()
{
    x86_outb(0x1f2, 0x01);
    x86_outb(0x1f3, 0);
    x86_outb(0x1f4, 0);
    x86_outb(0x1f5, 0);
    x86_outb(0x1f6, 0xe0);
    x86_outb(0x1f7, 0x20);
    while( (x86_inb(0x1f7) & 0x88) != 0x08);
    
}


void main(void)
{
    int a = 0;
    ter_clear();
    ter_show("Hello World\n", TER_RED);
    ter_show("This is from C\n", TER_BLUE);
    ter_setcur(24, 0);
    ter_show("Nice\n", TER_GREEN);
    ter_show("Good", TER_GREEN | TER_BLUE);
    ter_show("Wonderful\n", TER_GREEN | TER_BLUE);

    test_inoutb();

    while(1);
}
