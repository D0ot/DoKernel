#include <stddef.h>
#include <stdint.h>
#include "terminal.h"
#include "x86.h"
#include "disk.h"
#include "common.h"

char *buffer = (char*)0x10000;

void main(void)
{
    ter_clear();
    ter_show("Hello World\n", TER_RED);
    ter_show("This is from C\n", TER_BLUE);
    ter_setcur(24, 0);
    ter_show("Nice\n", TER_GREEN);
    ter_show("Good", TER_GREEN | TER_BLUE);
    ter_show("Wonderful\n", TER_GREEN | TER_BLUE);

    printf("%x%x\n", 0x11223344, 0xffffffff);
    memset(buffer, '\n', 32);
    memcpy(buffer + 32, buffer, 32);

    printf("length of \"%s\" : %d\n", "hello", strlen("hello"));
    strcpy(buffer, "Hello World\n");
    printf(buffer);
    






    while(1);
}
