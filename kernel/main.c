#include <stddef.h>
#include <stdint.h>
#include "terminal.h"

char * hello = "Hello World\n";

void main(void)
{
    ter_clear();
    ter_show(hello, TER_RED);
    ter_show("This is from C", TER_BLUE);
    ter_setcur(24, 0);
    ter_show("Nice\n", TER_GREEN);
    ter_show("Good\n", TER_GREEN | TER_BLUE);
    while(1);
}