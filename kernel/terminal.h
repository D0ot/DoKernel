#ifndef _TERMINAL_H
#define _TERMINAL_H

#include <stdint.h>
#include <stddef.h>

#define TER_RED 0x04
#define TER_GREEN 0x02
#define TER_BLUE 0x01

void ter_putchar(char ch, char color);
void ter_setcur(uint8_t row, uint8_t col);
void ter_show(const char *msg, const char color);
void ter_scroll();
void ter_clear();

#endif 