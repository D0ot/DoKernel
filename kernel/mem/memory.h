#ifndef _MEMORY_H
#define _MEMORY_H

#include <stdint.h>
#include <stddef.h>
#include "arch/x86/x86.h"

typedef struct AddressRangeDescriptorStructure_tag
{
    uint32_t base_address_low;
    uint32_t base_address_high;

    uint32_t length_low;
    uint32_t length_high;

    uint32_t type;
    uint32_t acpi3_extended;

} __attribute__((packed)) Mem_SMAP_Entry;

void memory_init(const Mem_SMAP_Entry* smap, uint32_t size);

#endif