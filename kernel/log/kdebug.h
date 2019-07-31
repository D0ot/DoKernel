#ifndef _KDEBUG_H
#define _KDEBUG_H

#include <stdarg.h>

enum 
{
    LOG_LEVEL_DEBUG = 0,
    LOG_LEVEL_INFO,
    LOG_LEVEL_WARNING,
    LOG_LEVEL_ERROR
};

#define LOG_LEVEL(...)                          \
    do                                          \
    {                                           \
        log("%s:%d | ", __FILE__, __LINE__);    \
        log_level(__VA_ARGS__);                 \
        log_putchar('\n');                      \
    }while(0)

#define LOG_DEBUG(...) LOG_LEVEL(LOG_LEVEL_DEBUG, __VA_ARGS__);
#define LOG_INFO(...) LOG_LEVEL(LOG_LEVEL_INFO, __VA_ARGS__);
#define LOG_WARNING(...) LOG_LEVEL(LOG_LEVEL_WARNING, __VA_ARGS__);
#define LOG_ERROR(...) LOG_LEVEL(LOG_LEVEL_ERROR, __VA_ARGS__);


void log_putchar(char ch);
void log(const char *format, ...);
void log_level(int level, const char *format, ...);


#endif 