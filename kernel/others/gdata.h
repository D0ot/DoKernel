#ifndef _GDATA_H
#define _GDATA_H

#include <stdint.h>
#include "mem/memory.h"
#include "defs/bios_conv.h"


#define GLOBAL_DATA_SIZE (0x300000)

typedef struct Global_Data_Structure_tag
{
    // Main Memory Region
    Memory_Region mmr;

    // GDT stuff
    uint16_t gdt_limit;
    uint32_t *gdt_addr;
    
    uint32_t gdt_entries[16];
    

} __attribute__((packed)) Global_Data_Struct;   

// size of it cant exceed 3Mib


Global_Data_Struct* global_data = (Global_Data_Struct*)(MAIN_PHYSICAL_MEMORY_START);

#endif 