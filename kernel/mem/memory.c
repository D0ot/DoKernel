#include <stdint.h>
#include <stddef.h>
#include "mem/memory.h"
#include "arch/x86/x86.h"
#include "driver/uart.h"
#include "common/common.h"
#include "log/kdebug.h"
#include "others/gdata.h"


void memory_init(const Mem_SMAP_Entry* smap, uint32_t size)
{
    LOG_INFO("memory_init start.");
    LOG_INFO("smap : %x, size : %d", smap, size);
    uint32_t max_usable_length_index = 0;
    uint8_t exist_usable_memory = 0;
    for(uint32_t i = 0; i < size; ++i)
    {
        LOG_INFO("%dth entry in smap: %x%x, %x%x, %x, %x", i,  
            smap[i].base_address_high, 
            smap[i].base_address_low, 
            smap[i].length_high, 
            smap[i].length_low, 
            smap[i].type, 
            smap[i].acpi3_extended);

        if(smap[i].type == MEM_SMAP_TYPE_USABLE)
        {
            // find the memory region with maximum length
            if(exist_usable_memory)
            {
                if(smap[i].length_high > smap[max_usable_length_index].length_high)
                {
                    max_usable_length_index = i;
                }
                else if(smap[i].length_high == smap[max_usable_length_index].length_high &&
                    smap[i].length_low > smap[max_usable_length_index].length_low)
                {
                    max_usable_length_index = i;
                }

            } else
            {
                // set flag, indicating there are usable memory.
                exist_usable_memory = 1; 
                // it not final result, here it is just a init.
                max_usable_length_index = i;

            }
        }

    }

    if(!exist_usable_memory)
    {
        LOG_ERROR("Not found available memory region, enter error loop.");
        while(1);
    }

    LOG_INFO("Index of memory region with maximum length : %d", max_usable_length_index);

    global_data->mmr.base = ((smap[max_usable_length_index].base_address_high << 32) |
                             (smap[max_usable_length_index].base_address_low)) + KERNEL_SIZE + GLOBAL_DATA_SIZE;
    
    global_data->mmr.length = (smap[max_usable_length_index].length_high << 32) |
                              (smap[max_usable_length_index].length_low);
    

    // move gdt to global_data
    global_data->gdt_addr = &(global_data->gdt_entries[0]);
    global_data->gdt_limit = 3 * 8 - 1;
    memcpy(&(global_data->gdt_entries[0]), (void*)(0x7e00), 8 * 3);
    x86_lgdt(&(global_data->gdt_limit));

    
    LOG_INFO("memory_init end");
}