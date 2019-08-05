#ifndef _ACPI_H
#define _ACPI_H

#include <stdint.h>

#define PTR_2_EBDA (0x40E)

#define ANOTHER_RSDP_START  (0xE0000)
#define ANOTHER_RSDP_END    (0x100000)



// structure defination

typedef struct RSDP_Struct_Tag
{
    char signature[8];
    uint8_t checksum;
    uint8_t oemid[6];
    uint8_t revision;
    void *rsdtAddress;

    // following is for Revision value is 2 or above
    // reference ACPI Spec6.3, 5.2.5.3
    uint64_t XsdtAddress; // this is a pointer
    uint8_t extendedchecksum;
    uint8_t reversed[3];

} __attribute__((packed)) RSDP_Struct;



uint8_t *findRSDPaddress();
RSDP_Struct getRSDP_Struct(uint8_t *address);



#endif 