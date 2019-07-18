#ifndef _X86_H
#define _X86_H

#include <stdint.h>

void x86_outb(uint16_t port, uint8_t data)
{
    uint16_t p = port;
    uint8_t d = data;
    asm volatile ("out %0, %1;" : : "a"(d), "d"(p));
}

static inline uint8_t x86_inb(uint16_t port)
{
    uint8_t temp;
    asm volatile ("in %1, %0;" : "=a"(temp) : "d"(port));
    return temp;
}

static inline uint16_t x86_inw(uint16_t port)
{
    uint16_t temp;
    asm volatile ("in %1, %0;": "=a"(temp) : "d"(port));

    return temp;

}

static inline uint16_t x86_outw(uint16_t port)
{
    return port;
}





#endif