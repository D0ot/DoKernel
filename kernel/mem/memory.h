#ifndef _MEMORY_H
#define _MEMORY_H

#include <stdint.h>
#include <stddef.h>
#include "arch/x86/x86.h"

#define MEM_SMAP_TYPE_USABLE (1)
#define MEM_SMAP_TYPE_RESERVED (2)

#define PAGE_SIZE (4096)
#define BUDDY_MAX_LEVEL (15)








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

    // the size of every buddy block is (4K-Byte * level^2)
    uint8_t level;


    // if is used , set the flags
    // valid for first Buddy_Element in Buddy Block
    // set flag in alloc operation
    // clear flag in free operation
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

typedef struct Buddy_Control_tag
{
    uint32_t base;
    uint32_t length;
    Buddy_Element *buddies_ptr;
    uint32_t buddies_number;
} Buddy_Control;


void memory_init(const Mem_SMAP_Entry* smap, uint32_t size);

// in memory_asm.asm
void memory_switch_gdt(void *gdt_addr);
void memory_init_flush_0();


// Inltialization of Buddy System
void buddy_init(Buddy_Control *bc, uint32_t base, uint32_t length, Buddy_Element *buddies_ptr);




// ------------------- allocation operations ---------- 
Buddy_Block buddy_alloc_bypage(Buddy_Control *bc, uint32_t page_count);
Buddy_Block buddy_alloc_bylevel(Buddy_Control *bc, uint8_t level);

/**
 *  \brief Allocate a Buddy Block
 *  \param bc
 *  \param index, where to allocate
 *  \param level, which level to allocate
 *      Caller must ensure 
 *          buddies_ptr[index].in_info_block == 1
 *          buddies_ptr[index].used == 0
 *      callee will check it, or allocating will fail.
 *      
 *      and spliting a Buddy can happed when level is smaller than bc->buddies_ptr[index].level
 */
Buddy_Block buddy_alloc_backend(Buddy_Control *bc, uint32_t index, uint8_t level);


/**
 *  \brief Allocate a Buddy Block at specified address
 *  \param bc, control structure
 *  \param addr, specified address
 *  \param level, the level of buddy
 *      carefully use it.
 */


Buddy_Block buddy_alloc_byaddr(Buddy_Control *bc, void* addr, uint8_t level);






// ---------- free operations ----------
uint8_t buddy_free_byindex(Buddy_Control *bc, uint32_t index);
uint8_t buddy_free_byaddr(Buddy_Control *bc, void *addr);
uint8_t buddy_free_backend(Buddy_Control *bc, void *addr); // same for free_byaddr, not impelemented




// ---------- aux functions ---------- 
// for the function below
// real memory operation not involve, purely logical operation
// or we can say, they are computation
uint8_t buddy_find_level(uint32_t page_count);
void *buddy_get_addr_byindex(Buddy_Control *bc, uint32_t buddy_ele_index);
uint32_t buddy_get_size_byindex(Buddy_Control *bc, uint32_t buddy_ele_index);
uint32_t buddy_get_index_byaddr(Buddy_Control *bc, void *addr);


/**
 *  \brief find the info block the index buddy_element
 *  \return infoblock's index
 *      if value returned is same to index, check if index is infoblock by yourself
 *      for this situation could indicate that not found.
 */
uint32_t buddy_get_infoblock_byindex(Buddy_Control *bc, uint32_t index);



// for the function below
// debug purpose
void buddy_debug_show(Buddy_Control *bc);

#endif