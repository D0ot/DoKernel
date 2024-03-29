#include <stddef.h>
#include <stdint.h>
#include "driver/terminal.h"
#include "arch/x86/x86.h"
#include "arch/x86/paging.h"
#include "driver/disk.h"
#include "common/common.h"
#include "driver/uart.h"
#include "log/kdebug.h"
#include "mem/memory.h"
#include "defs/bios_conv.h"
#include "others/gdata.h"
#include "driver/acpi.h"

#include "test/test_mem.h"

void main(void);
void exit(void);


void main(void)
{
    uart_init();
    ter_init();
    memory_init(SMAP_ADDR, SMAP_SIZE);

    buddy_debug_show(&global_data->kernel_mem);



    exit();
}

void exit(void)
{
    LOG_INFO("Entering exit loop");
    while(1);
}
