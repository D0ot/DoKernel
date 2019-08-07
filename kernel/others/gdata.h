#ifndef _GDATA_H
#define _GDATA_H

#include <stdint.h>
#include "mem/memory.h"
#include "arch/x86/paging.h"
#include "defs/bios_conv.h"


#define GLOBAL_DATA_SIZE (0x300000)
#define GLOBAL_DATA_OFFSET (0x100000)

typedef struct Global_Data_Structure_tag
{
    // Page Directory Entries
    Page_Directory_Entry pdes[1024];
    
    // Main Memory Region
    // kerne not contained
    // gdata not contained
    // Buddy elements not contained
    Memory_Region mmr;

    // GDT stuff
    uint16_t gdt_limit;
    uint32_t *gdt_addr;
    
    uint32_t gdt_entries[32];
    
    // for physical memory
    Buddy_Control physical_mem;


    // for kernel linear address
    // this is a address space for kernel
    Buddy_Control kernel_mem;


    
    Buddy_Block kernel_mem_bb_in_phy;

    

} __attribute__((packed)) Global_Data_Struct;   

// size of it cant exceed 3Mib


extern Global_Data_Struct* global_data;

#endif 