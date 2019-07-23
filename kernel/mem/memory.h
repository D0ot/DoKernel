#ifndef _MEMORY_H
#define _MEMORY_H

#include <stdint.h>
#include <stddef.h>
#include "arch/x86/x86.h"

#define MEM_SMAP_TYPE_USABLE (1)
#define MEM_SMAP_TYPE_RESERVED (2)


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
    uint64_t base;  

    // length
    uint64_t length;
} Memory_Region;

void memory_init(const Mem_SMAP_Entry* smap, uint32_t size);

#endif