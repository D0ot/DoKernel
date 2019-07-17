#ifndef _TERMINAL_H
#define _TERMINAL_H

#include <stdint.h>
#include <stddef.h>
    

void ter_putchar(char ch, char color);
void ter_setcur(uint8_t row, uint8_t col);
void ter_show(const char *msg);


#endif 