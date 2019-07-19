#include <stddef.h>
#include <stdint.h>
#include "terminal.h"
#include "x86.h"
#include "disk.h"
#include "common.h"
#include "uart.h"

char *buffer = (char*)0x10000;

void main(void)
{
    uart_init();
    ter_init();
    ter_setcolor(TER_RED);
    printf("Hello World\n");

    while(1);
}
