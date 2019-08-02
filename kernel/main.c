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

    uint32_t page_counts[] = {
        11, 33, 55, 77, 99, 22, 44, 66, 88, 1000, 
        33, 55, 1024, 33, 9, 7, 1, 3, 4, 5, 6, 7,
        1000, 3000, 50, 1, 1, 1, 1, 1, 1, 1, 1};

    Buddy_Block bbs[100];

    buddy_debug_show();
    for(uint8_t i = 0; i < sizeof(page_counts)/sizeof(*page_counts); ++i)
    {
        bbs[i] = buddy_alloc_bypage(page_counts[i]);
        LOG_DEBUG("After Alloc %d pages at 0x%x", page_counts[i], bbs[i].addr);
        buddy_debug_show();
    }


    buddy_debug_show();
    for(uint8_t i = 0; i < sizeof(page_counts)/sizeof(*page_counts); ++i)
    {
        buddy_free_byaddr(bbs[i].addr);
        LOG_DEBUG("After free pages at 0x%x", bbs[i].addr);
        buddy_debug_show();
    }




    exit();
}

void exit(void)
{
    LOG_INFO("Entering exit loop");
    while(1);
}
