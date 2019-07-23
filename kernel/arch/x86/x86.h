#ifndef _X86_H
#define _X86_H

#include <stdint.h>

// here starts the inline asm functions


static inline uint8_t x86_inb(uint16_t port)
{
    uint8_t data;
    asm volatile ("in %0, %1;" : "=a"(data) : "d"(port));
    return data;
}

static inline void x86_outb(uint16_t port, uint8_t data)
{
    asm volatile ("out %1, %0;" : : "a"(data), "d"(port));
}

static inline uint16_t x86_inw(uint16_t port)
{
    uint16_t data;
    asm volatile ("in %0, %1;": "=a"(data) : "d"(port));
    return data;
}

static inline void x86_outw(uint16_t port, uint16_t data)
{
    asm volatile ("out %1, %0;" : : "a"(data), "d"(port));
}

static inline void x86_insw(uint16_t port, uint16_t *buf, uint32_t count_byte)
{
    asm volatile (
        "cld;"
        "rep insw;"
        : "=D"(buf), "=c"(count_byte) 
        : "d"(port), "0"(buf), "1"(count_byte)
        : "memory", "cc"
    );
}

static inline void x86_lgdt(void *addr)
{
    asm volatile (
        "lgdt [%0];"
        : /* No output */
        : "a"(addr)
    );
}




#endif