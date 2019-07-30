#ifndef _COMMON_H
#define _COMMON_H
#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>

typedef void(*out_func_ptr)(char);

void putchar(char ch);

int sprintf(char *str, const char* format, ...);
int printf(const char* format, ...);

int v_sprintf(char *str, const char* format, va_list ap);
int v_printf(const char *format, va_list ap);
int v_printf_callback(const char*format,out_func_ptr out_func, va_list ap);

void *memset(void *ptr, char value, uint32_t num);
void *memcpy(void *dest, const void *src, size_t num);
void *memmove(void *dest, const void *src, size_t num);
char *strcpy(char *dest, const char *src);
size_t strlen(const char* str);

int abs(int value);
void swap(char *x, char *y);
char* reverse(char *buffer, size_t i, size_t j);
char *itoa(int value, char *str, int base);
char *utoa(unsigned value, char *str, unsigned base);


uint32_t poweru32(uint32_t b, uint32_t p);

#endif 