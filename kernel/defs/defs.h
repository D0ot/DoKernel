#ifndef _DEFS_H
#define _DEFS_H

// content here is same with content in boot/defs/asm

#define HIGH_BASE (0x80000000)

#define MEMINFO_OFFSET (0x8000)
#define KERNEL_OFFSET (0x400000)



#define MEMINFO_ADDR (HIGH_BASE + MEMINFO_OFFSET)
#define KERNEL_ADDR (HIGH_BASE + MEMINFO_OFFSET)

#define ARDS_SIZE (0x18)
#define LIB_ADDR (0x9000)

#define BUDDY_SYSTEM_SIZE (0x400000)

#endif
