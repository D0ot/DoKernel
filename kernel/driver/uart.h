#ifndef _UART_H
#define _UART_H

#include <stdint.h>

#define UART_COM1 (0x3F8)
#define UART_COM2 (0x2F8)

#define UART_DATA (0x0)     // data register.
#define UART_DIVL (0x0)     // with DLAB set to 1, least significant byte of  
                            // divisior for baud rate.

#define UART_DIVM (0x1)     // with DLAB set to 1, most significant byte.   
#define UART_INTE (0x1)     // interrupt Enable register

#define UART_INTI (0x2)     // interrupt identification 

#define UART_FCRL (0x2)     // FIFO control registers

#define UART_LCRL (0x3)     // Line Control Register
                            // most significant bit of this register is DLAB

#define UART_MCRL (0x4)     // Modern Control Register

#define UART_LSTA (0x5)     // Line status Register

#define UART_MSTA (0x6)     // Modern status Register

#define UART_SCRA (0x7)     // Scratch Register


void uart_init();

void uart_putchar(char ch);

uint8_t uart_is_empty();

char uart_getchar();

#endif