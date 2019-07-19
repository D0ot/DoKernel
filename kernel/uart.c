#include "uart.h"
#include "x86.h"

void uart_init()
{
    x86_outb(UART_COM1 + UART_INTE, 0x00); // disable all intrrupt
    x86_outb(UART_COM1 + UART_LCRL, 0x80); // Set DLAB
    x86_outb(UART_COM1 + UART_DIVL, 0x03); // set least significant byte of divisor to 0x01
    x86_outb(UART_COM1 + UART_DIVM, 0x00); // set most significant byte of divisor to 0
    x86_outb(UART_COM1 + UART_LCRL, 0x03); // 8 bits, no parity, one stop bit
    x86_outb(UART_COM1 + UART_FCRL, 0xc7); // disable FIFO
    x86_outb(UART_COM1 + UART_MCRL, 0x0b); // IRQs enabled, RTS/DSR set

}

uint8_t uart_is_empty()
{
    return x86_inb(UART_COM1 + UART_LSTA) & 0x20;
}


void uart_putchar(char ch)
{
    while(uart_is_empty() == 0);
    x86_outb(UART_COM1 + UART_DATA, ch);

}

char uart_getchar()
{

}
