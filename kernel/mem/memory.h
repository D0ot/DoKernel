#ifndef _MEMORY_H
#define _MEMORY_H

#include <stdint.h>
#include <stddef.h>
#include "arch/x86/x86.h"

#define MEM_SMAP_TYPE_USABLE (1)
#define MEM_SMAP_TYPE_RESERVED (2)

#define PAGE_SIZE 4096






typedef struct AddressRangeDescriptorStructure_tag
{
    uint32_t base_address_low;
    uint32_t base_address_high;

    uint32_t length_low;
    uint32_t length_high;

    uint32_t type;
    uint32_t acpi3_extended;

} __attribute__((packed)) Mem_SMAP_Entry;

// any Memory_Region is all usable.
typedef struct Memory_Region_tag
{
    // base address
    uint32_t base;  

    // length
    uint32_t length;
} Memory_Region;


// for buddy system
typedef struct Buddy_Element_tag
{
    // used to specify the block at left or right at a level
    // BIT[15...0], bit 0 is left-right state of level 0.
    // 0 for left, 1 for right
    uint16_t lr;

    // the size of every buddy block is (4K-Byte * level^2)
    uint8_t level;


    // if is used , set the flags
    // valid for first Buddy_Element in Buddy Block
    uint8_t used : 1;

    // is it a block containing infomation?
    // if set, it contains infomation.
    uint8_t is_info_block: 1;

    // for later use...
    uint8_t reserved:6;
} __attribute__((packed)) Buddy_Element;


// the responsive struct for any memory request for buddy system
typedef struct Buddy_Block_tag
{
    void* addr;
    uint32_t size;
} Buddy_Block;


void memory_init(const Mem_SMAP_Entry* smap, uint32_t size);

// in memory_asm.asm
void memory_switch_gdt(void *gdt_addr);
void memory_init_flush_0();


// Inltialization of Buddy System
void buddy_init();

// allocation operations
Buddy_Block buddy_alloc_bypage(uint32_t page_count);
void* buddy_alloc_bylevel(uint8_t level);
void* buddy_alloc_byaddr(void* addr);

// free operations
uint8_t buddy_free_byindex(uint32_t index);
uint8_t buddy_free_byaddr(void* addr);

// for the function below
// real memory operation not involve, purely logical operation
// or we can say, they are computation
uint8_t buddy_find_level(uint32_t page_count);
void* buddy_get_addr_byindex(uint32_t buddy_ele_index);
uint32_t buddy_get_size_byindex(uint32_t buddy_ele_index);



// for the function below
// debug purpose
void buddy_debug_show();

#endif