#include "log/kdebug.h"
#include "driver/terminal.h"
#include "common/common.h"
#include "driver/uart.h"
#include <stdarg.h>


void log_putchar(char ch)
{
    uart_putchar(ch);
}

void log(const char *format, ...)
{
    va_list ap;
    va_start(ap, format);
    v_printf_callback(format, log_putchar, ap);
    va_end(ap);
}

void log_level(int level, const char *format, ...)
{
    va_list ap;
    va_start(ap, format);
    if(level < 0 || level > 3)
    {
        while(1)
        {
            // usage error.
        }
    }
    char *level_str[4] = {"Debug", "Info", "Warning", "Error"};
    char *output = level_str[level];
    while(*output)
    {
        log_putchar(*output++);
    }
    output = " | ";
    while (*output)
    {
        log_putchar(*output++);
    }
    
    v_printf_callback(format, log_putchar, ap);
    va_end(ap);
}