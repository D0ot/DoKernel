#include <stddef.h>
#include <stdint.h>
#include "terminal.h"
#include "x86.h"
#include "disk.h"
#include "common.h"
#include "uart.h"
#include "kdebug.h"
#include "memory.h"
#include "bios_conv.h"

void exit(void);

void main(void)
{
    uart_init();
    ter_init();
    memory_init(SMAP_ADDR, SMAP_SIZE);

    exit();
}

void exit(void)
{
    LOG_INFO("Entering exit loop");
    while(1);
}
