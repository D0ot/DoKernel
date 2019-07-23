
// bios_conv stands for bios convention, it connects kernel and loader


#ifndef _BIOS_CONV_H 
#define BIOS_CONV_H

#include "stdint.h"
#include "mem/memory.h"

// it is not a good design
// TODO, using a more "smart" loader
#define MAIN_PHYSICAL_MEMORY_START (0x100000) 

// address to which boot.asm load the program 
#define KERNEL_BASE (void*)(0x400000)

// not using ELF format, so hardcode the max image size
// for learning purpose, 4Mib will be enough
#define KERNEL_SIZE (uint32_t)(0x400000) 


#define SMAP_BASE ((void*)0x8000)
#define SMAP_SIZE (*(uint32_t*)(SMAP_BASE))
#define SMAP_ADDR ((Mem_SMAP_Entry*)(SMAP_BASE + 4))



#endif