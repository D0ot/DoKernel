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
    LOG_INFO("{memory_init start.");
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




    // perpare a page frame to store buddies about physical memory

    // memory the buddy system itself consumes, is not managed by buddy system

    // add a Entry in Page Directory to store buddies element
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

    Buddy_Element *physical_mem = 0x800000;

    memset(physical_mem, 0, 1024*4096); //clear the new 4M page

    buddy_init(&global_data->physical_mem, global_data->mmr.base, global_data->mmr.length, physical_mem);


    
    LOG_INFO("memory_init end}");
}

void buddy_init(Buddy_Control *bc, uint32_t base, uint32_t length, Buddy_Element *buddies_ptr)
{

    // the max block of this impelementaion is 2^(BUDDY_MAX_LEVEL) Page 
    // BUDDY_MAX_LEVEL is in memory.h

    LOG_INFO("{ buddy_init start, base:0x%x, length:0x%x", base, length);
    uint32_t page_number = length / PAGE_SIZE;
    LOG_INFO("Avaliable page_nubmer : 0x%x", page_number);
    uint32_t mem_res = length % PAGE_SIZE;
    LOG_INFO("Abandoned memory, mem_res : 0x%x", mem_res);

    bc->buddies_number = page_number;
    bc->base = base;
    bc->buddies_ptr = buddies_ptr;



    for(uint32_t i = 0; i < page_number;)
    {
        uint32_t avaliable_now = page_number - i;
        uint8_t max_power = 0;
        while(poweru32(2, max_power) <= avaliable_now && (max_power <= BUDDY_MAX_LEVEL + 1))
        {
            ++max_power;
        }

        uint8_t found_buddy_level = max_power - 1;
        buddies_ptr[i].level = found_buddy_level;   // we found the highest level buddy from avaliable page!
        buddies_ptr[i].is_info_block = 1;           // indicating that it is a control block
        LOG_INFO("A buddy found at index : 0x%x, level : %d, addr : 0x%x ", i, found_buddy_level, buddy_get_addr_byindex(bc, i));
        
        i += poweru32(2, found_buddy_level);
    }

    LOG_INFO("buddy_init end }");
}

// it returns physical address and block size in Byte
// all zeor for failed allocation
Buddy_Block buddy_alloc_bypage(Buddy_Control *bc, uint32_t page_count)
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

    return buddy_alloc_bylevel(bc, level);
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

Buddy_Block buddy_alloc_bylevel(Buddy_Control *bc, uint8_t level)
{
    uint32_t buddies_number = bc->buddies_number;
    Buddy_Element * buddies_ptr = bc->buddies_ptr;
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
                //found the a buddy prefectly
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
                    }
                    iter += powerof2(buddies_ptr[iter].level);

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


    if(iter >= buddies_number && !min_index_init_guard)
    {
        // min_index_init_guard is not set
        // indiciating that we did not find any blocks larger than the requested one.
        // nothing is found
        LOG_WARNING("buddy_alloc_bylevel failed. level : 0x%x", level);
        Buddy_Block bb;
        bb.size = 0;
        return bb;
    }


    uint32_t to_alloc_index;

    if(iter < buddies_number)
    {
        to_alloc_index = iter;
    }
    else
    {
        to_alloc_index = min_index;
    }

    return buddy_alloc_backend(bc, to_alloc_index, level);
    

}

Buddy_Block buddy_alloc_backend(Buddy_Control *bc, uint32_t index, uint8_t level)
{
    Buddy_Element *buddies_ptr = bc->buddies_ptr;
    Buddy_Block bb;
    
    if(buddies_ptr[index].is_info_block == 0 || 
       buddies_ptr[index].used == 1)
    {
        // not a info_block
        LOG_ERROR("buddy_alloc_backend, arguemnt invalid.")
        bb.size = 0;
        return bb;
    }

    if(level > buddies_ptr[index].level)
    {
        LOG_ERROR("buddy_alloc_backend, argument invalid : level");
        bb.size = 0;
        return bb;
    }

    uint32_t iter = index;
    while(buddies_ptr[iter].level != level)
    {
        // now level
        uint8_t nl = buddies_ptr[iter].level;
        buddies_ptr[iter + powerof2(nl - 1)].is_info_block = 1;
        buddies_ptr[iter + powerof2(nl - 1)].level = nl - 1;
        buddies_ptr[iter].level = nl - 1;
    }
    buddies_ptr[index].used = 1;
    bb.addr = buddy_get_addr_byindex(bc, index);
    bb.size = buddy_get_size_byindex(bc, index);
    LOG_INFO("buddy_alloc_backend, bb.addr = 0x%x, bb.size = 0x%x", bb.addr, bb.size);
    return bb;
}

void* buddy_alloc_byaddr(Buddy_Control *bc, void* addr)
{
    LOG_ERROR("buddy_alloc_byaddr() not impelemented.");
    while(1)
    {
        // inf loop
    }
    
}

void* buddy_get_addr_byindex(Buddy_Control *bc, uint32_t buddy_ele_index)
{
    return (void*)(bc->base + PAGE_SIZE * buddy_ele_index);
}

uint32_t buddy_get_size_byindex(Buddy_Control *bc, uint32_t buddy_ele_index)
{
    return PAGE_SIZE * powerof2(bc->buddies_ptr[buddy_ele_index].level);
}


void buddy_debug_show(Buddy_Control *bc)
{
    Buddy_Element *buddies_ptr = bc->buddies_ptr;
    LOG_DEBUG("buddy_debug_show");
    for(uint32_t index = 0; index < bc->buddies_number;)
    {
        LOG_DEBUG
        (
            "index:0x%x level:%d used:%d",
            index, buddies_ptr[index].level, buddies_ptr[index].used
        );

        LOG_DEBUG
        (
            "addr:0x%x size:0x%x endaddr:0x%x",
            buddy_get_addr_byindex(bc, index),
            buddy_get_size_byindex(bc, index),
            buddy_get_addr_byindex(bc, index) + buddy_get_size_byindex(bc, index)
        );

        index += powerof2(buddies_ptr[index].level);
    }
}

uint8_t buddy_free_byindex(Buddy_Control *bc, uint32_t index)
{
    if(index >= bc->buddies_number)
    {
        LOG_ERROR("buddy_free_byindex, index:(0x%x) >= buddies_number", index);
        while(1);
    }

    Buddy_Element *buddies_ptr = bc->buddies_ptr;

    if(!buddies_ptr[index].used)
    {
        LOG_ERROR("buddy_free_byindex, free the unused index:(0x%x)", index);
        while(1);
    }

    // index of currently solving buddy element
    uint32_t current_index = index;
    while(1)
    {
        uint8_t level = buddies_ptr[current_index].level;
        uint8_t lr = (current_index & (1 << buddies_ptr[current_index].level));


        if(!lr)
        {
            // current_index is at left
            uint32_t ele_at_right = current_index + powerof2(level);

            // if ele_at_right satisfies the following three conditions
            // we can do the combinatin
            if(buddies_ptr[ele_at_right].is_info_block &&
               !buddies_ptr[ele_at_right].used &&
               buddies_ptr[ele_at_right].level == level)
            {
                buddies_ptr[current_index].level = level + 1;
                buddies_ptr[ele_at_right].is_info_block = 0;
                buddies_ptr[ele_at_right].used = 0;
                
                // we dont have to update current_index
                // for we are the left
            }
            else
            {
                // combination can not continue
                // set used flag to 0
                buddies_ptr[current_index].used = 0;
                break;
            }
            

        } 
        else
        {
            // current_index is at right
            uint32_t ele_at_left = current_index - powerof2(level);

            if(buddies_ptr[ele_at_left].is_info_block &&
               !buddies_ptr[ele_at_left].used &&
               buddies_ptr[ele_at_left].level == level)
            {
                buddies_ptr[ele_at_left].level = level + 1;
                buddies_ptr[current_index].is_info_block = 0;
                buddies_ptr[current_index].used = 0;

                current_index = ele_at_left;
            }
            else
            {
                buddies_ptr[current_index].used = 0;
                break;
            }

        }
    }


    return 0;    
}

uint32_t buddy_get_infoblock_byindex(Buddy_Control *bc, uint32_t index)
{
    for(uint8_t i = 0; i <= BUDDY_MAX_LEVEL; ++i)
    {
        uint32_t masked = index & (0xffffffff << i);
        if(bc->buddies_ptr[masked].is_info_block)
        {
            return masked;
        }
    }
    return index;

}

uint8_t buddy_free_byaddr(Buddy_Control *bc, void* addr)
{
    // convert addr to uint32_t for convenience
    int32_t diff = (uint32_t)addr - bc->base;

    if(diff < 0)
    {
        LOG_ERROR("buddy_free_byaddr, addr:(0x%x) not higher than base", addr);
        while(1);
    }


    if(diff % PAGE_SIZE != 0)
    {
        LOG_ERROR("buddy_free_byaddr, addr:(0x%x) not aligen to PAGESIZE", addr);
        while(1);
    }

    uint32_t buddy_index = diff / PAGE_SIZE;

    LOG_INFO("free index 0x%x", buddy_index);
    return buddy_free_byindex(bc, buddy_index);

}