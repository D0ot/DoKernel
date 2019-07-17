#include <stddef.h>
#include <stdint.h>
#include "terminal.h"

char * hello = "Hello World";

void main(void)
{
    ter_show(hello);
    ter_show("This is from C\n");
    ter_setcur(24, 0);
    ter_show("Nice\n");
    ter_show("Good\n");
    while(1);
}