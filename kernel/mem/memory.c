#include "mem/memory.h"
#include "arch/x86/x86.h"
#include "driver/uart.h"
#include "common/common.h"
#include "log/kdebug.h"


void memory_init(const Mem_SMAP_Entry* smap, uint32_t size)
{
    LOG_INFO("memory_init start.");
    LOG_INFO("smap : %x, size : %d", smap, size);
    for(uint32_t i = 0; i < size; ++i)
    {
        LOG_INFO("%dth entry in smap: %x%x, %x%x, %x, %x", i,  
            smap[i].base_address_high, 
            smap[i].base_address_low, 
            smap[i].length_high, 
            smap[i].length_low, 
            smap[i].type, 
            smap[i].acpi3_extended);
    }


    LOG_INFO("memory_init end");
}