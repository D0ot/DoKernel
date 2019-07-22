#include <stdint.h>
#include <stddef.h>
#include "driver/disk.h"
#include "arch/x86/x86.h"
#include "driver/terminal.h"

/*
    \brief reading disk.
    \param sector_index : from where to read
    \param count : how many sectors to read, 0 indicates 256
    \param buf : buffer
*/


void disk_read(uint32_t sector_index, uint8_t count, uint16_t *buf)
{
    x86_outb(ATA_PIO_MODE, ((sector_index >> 24) & 0x0f) | 0xe0);
    x86_outb(ATA_PIO_COUNT, count);
    x86_outb(ATA_PIO_LBA_LOW, sector_index & 0xff);
    x86_outb(ATA_PIO_LBA_MID, (sector_index >> 8) & 0xff);
    x86_outb(ATA_PIO_LAB_HIG, (sector_index >> 16) & 0xff);
    x86_outb(ATA_PIO_CMD, ATA_CMD_READ);

    uint8_t status;
    for(uint8_t i = 0; i < count; ++i)
    {
        while(1)
        {

            status = x86_inb(ATA_PIO_STATUS);
            if(status & STAT_BSY)
            {
                continue;
            }

            if(status & STAT_ERR)
            {
                ter_show("disk.c STAT_ERR\n", TER_RED);
                while (1);
            }

            if(status & STAT_DF)
            {
                ter_show("disk.c STAT_DF\n", TER_RED);
                while (1);
            }

            if(status & STAT_DRQ)
            {
                break;
            }

        }
        
        x86_insw(ATA_PIO_DATA, buf, 256);
        buf += 256;
    }

}