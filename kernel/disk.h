#ifndef _DISK_H
#define _DISK_H

#include <stddef.h>
#include <stdint.h>

#define STAT_ERR  (1 << 0) // Indicates an error occurred. Send a new command to clear it
#define STAT_DRQ  (1 << 3) // Set when the drive has PIO data to transfer, or is ready to accept PIO data.
#define STAT_SRV  (1 << 4) // Overlapped Mode Service Request.
#define STAT_DF   (1 << 5) // Drive Fault Error (does not set ERR).
#define STAT_RDY  (1 << 6) // Bit is clear when drive is spun down, or after an error. Set otherwise.
#define STAT_BSY  (1 << 7) // Indicates the drive is preparing to send/receive data (wait for it to clear).
                           // In case of 'hang' (it never clears), do a software reset.
//  7   6   5   4   3   2   1   0
//  BSY RDY DF  SRV DRQ         ERR

#define ATA_PIO_ERRCODE (0x1f1)
#define ATA_PIO_COUNT (0x1f2)
#define ATA_PIO_LBA_LOW (0x1f3)
#define ATA_PIO_LBA_MID (0x1f4)
#define ATA_PIO_LAB_HIG (0x1f5)
#define ATA_PIO_MODE (0x1f6)
#define ATA_PIO_STATUS (0x1f7)
#define ATA_PIO_CMD (0x1f7)
#define ATA_PIO_DATA (0x1f0)


#define ATA_CMD_READ (0x20)


void disk_read(uint32_t sector_index, uint8_t count, uint16_t *buf);
void disk_write(uint32_t sector_index, uint8_t count, uint16_t *buf); // not implemented
#endif