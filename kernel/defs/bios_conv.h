
// bios_conv stands for bios convention, it connects kernel and loader


#ifndef _BIOS_CONV_H 
#define _BIOS_CONV_H

#include <stdint.h>
#include "mem/memory.h"
#include "defs/defs.h"


// address to which boot.asm load the program 
#define KERNEL_BASE (void*)(KERNEL_ADDR)

// not using ELF format, so hardcode the max image size
// for learning purpose, 4MiB will be enough
#define KERNEL_SIZE (uint32_t)(0x400000) 


#define SMAP_BASE ((void*)MEMINFO_ADDR)
#define SMAP_SIZE (*(uint32_t*)(SMAP_BASE))
#define SMAP_ADDR ((Mem_SMAP_Entry*)(SMAP_BASE + 4))



#endif