#ifndef _BIOS_CONV_H 
#define BIOS_CONV_H

#include "stdint.h"
#include "mem/memory.h"

// bios_conv stands for bios convention

#define SMAP_BASE (0x8000)

#define SMAP_SIZE (*(uint32_t*)(SMAP_BASE))

#define SMAP_ADDR ((Mem_SMAP_Entry*)(SMAP_BASE + 4))

#endif