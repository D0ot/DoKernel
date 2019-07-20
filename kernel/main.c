#include <stddef.h>
#include <stdint.h>
#include "terminal.h"
#include "x86.h"
#include "disk.h"
#include "common.h"
#include "uart.h"
#include "kdebug.h"

char *buffer = (char*)0x10000;

void main(void)
{
    ter_init();
    printf("Terminal Init Done\n");
    LOG_DEBUG("Terminal Init Done");
    uart_init();
    LOG_DEBUG("Uart Init Done");
    printf("UART Init Done\n");

    exit();
}

void exit(void)
{
    LOG_DEBUG("Entering exit loop");
    while(1);
}
