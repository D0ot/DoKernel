#include <stddef.h>


void main(void)
{
    char *vram = (char*)0xB8000;
    vram[0] = 0x48;
    vram[1] = 0x07;

    while(1);
}