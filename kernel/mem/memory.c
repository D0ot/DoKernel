#include <stdint.h>
#include <stddef.h>
#include "mem/memory.h"
#include "arch/x86/x86.h"
#include "arch/x86/paging.h"
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

    global_data->mmr.base = (smap[max_usable_length_index].base_address_low) + KERNEL_SIZE + GLOBAL_DATA_SIZE + BUDDY_SYSTEM_SIZE;
    
    global_data->mmr.length = (smap[max_usable_length_index].length_low) - KERNEL_SIZE - GLOBAL_DATA_SIZE - BUDDY_SYSTEM_SIZE;
    

    // move gdt to global_data
    global_data->gdt_addr = &(global_data->gdt_entries[0]);
    global_data->gdt_limit = 3 * 8 - 1;
    memcpy(&(global_data->gdt_entries[0]), (void*)(0x7e00), 8 * 3);
    x86_lgdt(&(global_data->gdt_limit));
    memory_init_flush_0();

    buddy_init();


    
    LOG_INFO("memory_init end");
}

void buddy_init()
{
    // memory the buddy system itself consume, is not managed by buddy system

    // the max block of this impelementaion is 2^16 Page = 2^16 * 2^12 Byte = 2^28 Byte = 128M-Byte

    LOG_INFO("buddy_init start");
    uint32_t page_number = global_data->mmr.length / (1024*4);
    LOG_INFO("Avaliable page_nubmer : 0x%x", page_number);
    uint32_t mem_res = global_data->mmr.length % (1024*4);
    LOG_INFO("Abandoned memory, mem_res : 0x%x", mem_res);
    global_data->mmr.length -= mem_res; // some of the memory is "useless" for they are smaller than 4K-Byte(size of page).

    global_data->buddies_number = page_number;

    //add a Entry in Page Directory to store buddies element
    Page_Directory_Entry * pde_ptr = &(global_data->pdes[2]);
    memset(pde_ptr, 0, sizeof(Page_Directory_Entry));
    pde_ptr->p = 1;     // present
    pde_ptr->rw = 1;    // allow write to the page
    pde_ptr->us = 0;    // user-mode accessed not allowed
    pde_ptr->pwt = 0;   // disable write-through
    pde_ptr->pcd = 0;   // enable  cache
    pde_ptr->ps = 1;    // ps set, 4M page
    pde_ptr->address = ((uint32_t)0x02 << 10); // 0x800000 ~ 0xC00000
    // map (0x800000 ~ 0xC00000) to the same

    Buddy_Element *buddies_ptr = (Buddy_Element*)(0x800000);
    global_data->buddies_ptr = buddies_ptr;
    memset(buddies_ptr, 0, sizeof(Buddy_Element) * page_number); //clear the new 4M page

    for(uint32_t i = 0; i < page_number;)
    {
        uint32_t avaliable_now = page_number - i;
        uint8_t max_power = 0;
        while(poweru32(2, max_power) <= avaliable_now && (max_power <= 16))
        {
            ++max_power;
        }

        uint8_t found_buddy_level = max_power - 1;
        buddies_ptr[i].level = found_buddy_level;   // we found the highest level buddy from avaliable page!
        LOG_INFO("A buddy found at index : 0x%x, level : %d, addr : 0x%x ", i, found_buddy_level, buddy_get_addr_byindex(i));


        uint32_t endpos = poweru32(2, found_buddy_level) + i;
        // set left-right state of a buddy element
        for(int8_t iter_level = (found_buddy_level == 16 ? 15 : found_buddy_level); iter_level >= 0; --iter_level)
        {
            uint32_t iter = i;
            uint32_t step = poweru32(2, iter_level);
            uint8_t lr = 0; // 0 for left, 1 for right

            while(iter < endpos)
            {
                if(lr)
                {
                    buddies_ptr[iter].lr |= (1 << iter_level);
                } else
                {
                    buddies_ptr[iter].lr &= ~(1 << iter_level);
                }

                iter += step;
                lr = !lr;
            }

        }
        i += poweru32(2, found_buddy_level);
    }

    LOG_INFO("buddy_init end");
}

// it returns physical address and block size in Byte
// all zeor for failed allocation
Buddy_Block buddy_alloc_bypage(uint32_t page_count)
{
    uint8_t level = buddy_find_level(page_count);
    LOG_DEBUG("buddy level calculated is : %d", level);
    Buddy_Block bb;
    if(level == UINT8_MAX)
    {
        bb.addr = 0;
        bb.size = 0;
        return bb;
    }

    bb.addr = buddy_alloc_bylevel(level);
    bb.size = PAGE_SIZE * powerof2(level);
    return bb; 
}


// input page count, and return a suitable buddy element level value
// if cannot find a suitable level, returns UINT8_MAX
uint8_t buddy_find_level(uint32_t page_count)
{
    if(page_count == 0)
    {
        LOG_ERROR("buddy_find_level, page_count : 0x%x", page_count);
    }

    if(page_count == 1)
    {
        return 0; // level 0 is suitable
    }

    if(page_count > powerof2(16))
    {
        LOG_WARNING("buddy_find_level, a block request larger than 128M, page_nubmer : 0x%x", page_count);
        return UINT8_MAX;
    }

    uint8_t level = 1;
    for(; level <= 16; ++level)
    {
        if(powerof2(level - 1) < page_count && powerof2(level) >= page_count)
        {
            break;
        }
    }

    return level;
 
}

void* buddy_alloc_bylevel(uint8_t level)
{
    uint32_t buddies_number = global_data->buddies_number;
    Buddy_Element * buddies_ptr = global_data->buddies_ptr;
    uint32_t iter = 0;
    uint8_t min_index_init_guard = 0; // if set, indicating min_index in correctly inited.
    uint32_t min_index;     // the buddy element satisfy the following condition: 
                            //  1. level higher than argument level 
                            //  2. not used 
                            //  3. the min level one is 1 and 2.


    while(iter < buddies_number)
    {
        if(buddies_ptr[iter].used)
        {
            iter += powerof2(buddies_ptr[iter].level);
        } 
        else
        {
            if(level == buddies_ptr[iter].level)
            {
                //found the suitable prefectly
                break;
            } 
            else if(level > buddies_ptr[iter].level)
            {
                iter += powerof2(buddies_ptr[iter].level);
            }
            else if(level < buddies_ptr[iter].level)
            {
                // found a block which is larger than the requested and 
                if(min_index_init_guard)
                {
                    if(buddies_ptr[min_index].level > buddies_ptr[iter].level)
                    {
                        // the block found is smaller than min_index, replace min_index with new one.
                        min_index = iter;
                        iter += powerof2(buddies_ptr[iter].level);
                    }

                }else
                {
                    // min_index not inited, initialize it.
                    min_index = iter;
                    min_index_init_guard = 1;
                    iter += powerof2(buddies_ptr[iter].level);
                }

            }
        }

    }




    // prefectly match, use it
    if(iter < buddies_number && buddies_ptr[iter].level == level)
    {
        buddies_ptr[iter].used = 1;

        for(uint32_t i = 1; i < powerof2(level); ++i)
        {
            buddies_ptr[iter + i].level_used = 1;
        }
        
        // return a valid addres
        return buddy_get_addr_byindex(min_index); 
    }

    if(iter >= buddies_number && !min_index_init_guard)
    {
        // min_index_init_guard is not set
        // indiciating that we did not find any blocks larger than the requested one.
        // nothing is found
        LOG_WARNING("buddy_alloc_bylevel failed. level : 0x%x", level);
        return 0;
    }

    // we have to split a block to get suitable block

    for(uint8_t split_times = buddies_ptr[min_index].level - level; split_times; --split_times)
    {
        // do split sutff
        uint8_t new_level = buddies_ptr[min_index].level - 1;
        buddies_ptr[min_index].level = new_level;
        buddies_ptr[min_index + powerof2(new_level)].level = new_level;
    }

    
    for(uint32_t i = 1; i < powerof2(level); ++i)
    {
        buddies_ptr[min_index + i].level_used = 1;
    }
    buddies_ptr[min_index].used = 1; 

    return buddy_get_addr_byindex(min_index);
}

void* buddy_alloc_byaddr(void* addr)
{
    LOG_ERROR("buddy_alloc_byaddr() not impelemented.");
    while(1)
    {
        // inf loop
    }
    
}

void* buddy_get_addr_byindex(uint32_t buddy_ele_index)
{
    return (void*)(global_data->mmr.base + PAGE_SIZE * buddy_ele_index);
}

uint32_t buddy_get_size_byindex(uint32_t buddy_ele_index)
{
    return PAGE_SIZE * powerof2(global_data->buddies_ptr[buddy_ele_index].level);

}


void buddy_debug_show()
{
    Buddy_Element *buddies_ptr = global_data->buddies_ptr;
    LOG_DEBUG("buddy_debug_show");
    for(uint32_t index = 0; index < global_data->buddies_number;)
    {
        LOG_DEBUG
        (
            "index:0x%x level:%d used:%d lr:0x%x",
            index, buddies_ptr[index].level, buddies_ptr[index].used, buddies_ptr[index].lr
        );

        LOG_DEBUG
        (
            "addr:0x%x size:0x%x endaddr:0x%x",
            buddy_get_addr_byindex(index),
            buddy_get_size_byindex(index),
            buddy_get_addr_byindex(index) + buddy_get_size_byindex(index)
        );

        index += powerof2(buddies_ptr[index].level);
    }
}

uint8_t buddy_free_byindex(uint32_t index)
{
    if(index >= global_data->buddies_number)
    {
        LOG_ERROR("buddy_free_byindex, index:(0x%x) >= buddies_number", index);
        while(1);
    }

    Buddy_Element *buddies_ptr = global_data->buddies_ptr;

    if(buddies_ptr[index].used)
    {
        LOG_ERROR("buddy_free_byindex, free the unused index:(0x%x)", index);
        while(1);
    }

    uint8_t c = 1;
    while(c)
    {
        uint8_t level = buddies_ptr[index].level;
        uint8_t lr = (buddies_ptr[index].lr >> level) & 0x01;
        if(lr)
        {
            // at left
            


        } 
        else
        {
            // at right

        }
    }


    
}


uint8_t buddy_free_byaddr(void* addr)
{
    // convert addr to uint32_t for convenience
    int32_t diff = (uint32_t)addr - global_data->mmr.base;

    if(diff < 0)
    {
        LOG_ERROR("buddy_free_byaddr, addr:(0x%x) not higher than mmr.base", addr);
        while(1);
    }


    if(diff % PAGE_SIZE != 0)
    {
        LOG_ERROR("buddy_free_byaddr, addr:(0x%x) not aligen to PAGESIZE", addr);
        while(1);
    }

    uint32_t buddy_index = diff / PAGE_SIZE;

    return buddy_free_byindex(buddy_index);

}