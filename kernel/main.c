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

void main(void);
void exit(void);



void main(void)
{
    uart_init();
    ter_init();
    memory_init(SMAP_ADDR, SMAP_SIZE);

    buddy_debug_show();
    Buddy_Block bb =  buddy_alloc_bypage(1);
    LOG_DEBUG("get a 1 page : 0x%x, addr : 0x%x ", bb.size, bb.addr);
    buddy_debug_show();
    buddy_alloc_bypage(3);
    buddy_debug_show();

    for(uint32_t i = 0; i < global_data->buddies_number; ++i)
    {
        LOG_DEBUG("buddies_ptr[%d].lr = %d", i, global_data->buddies_ptr[i].lr);
    }


    exit();
}

void exit(void)
{
    LOG_INFO("Entering exit loop");
    while(1);
}
