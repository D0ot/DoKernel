#include <stdint.h>
#include "common/common.h"
#include "acpi.h"
#include "driver/uart.h"
#include "log/kdebug.h"

uint8_t *findRSDPaddress()
{
    uint8_t *ebda_addr = (uint8_t*)*(uint16_t*)(PTR_2_EBDA);

    for(int i = 0; i != 0x400; i += 0x10)
    {
        if(strmatch(ebda_addr + i, "RSD PTR ", 8))
        {
            return (ebda_addr + i);
        }
    }

    for(uint8_t *p = ANOTHER_RSDP_START; p != (uint8_t*)(ANOTHER_RSDP_END); p += 0x10)
    {
        if(strmatch(p, "RSD PTR ", 8))
        {
            return p;
        }
    }


    LOG_WARNING("Not found Address of RSDP");
    return (uint8_t*)0;

}


RSDP_Struct getRSDP_Struct(uint8_t *address)
{
    RSDP_Struct rsdp;
    // copy first 20 bytes for revision below 2.0
    memcpy((void*)&rsdp, address, 20);

    // calcuate the first checksum

    if(checksum(&rsdp, 20) != 0)
    {
        LOG_WARNING("checksum error, RSDP address : 0x%x", address);
        rsdp.rsdtAddress = 0;
        return rsdp;
    }

    // qemu's default revison is 0 
    if(rsdp.revision >= 2)
    {
        // copy the rest bytes
        memcpy((void*)&rsdp + 20, address + 20, 16);

        if(checksum(&rsdp, sizeof(rsdp)) != rsdp.extendedchecksum)
        {
            LOG_WARNING("checksum error, XSDP address : 0x%x", address);
            rsdp.rsdtAddress = 0;
            rsdp.XsdtAddress = 0;
            return rsdp;
        }


    }

    return rsdp;

}