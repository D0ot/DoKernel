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



int strcmp(char *s1, char *s2);
size_t strlen(const char* str);

int abs(int value);
void swap(char *x, char *y);
char* reverse(char *buffer, size_t i, size_t j);
char *itoa(int value, char *str, int base);
char *utoa(unsigned value, char *str, unsigned base);


uint32_t powerof2(uint8_t p);
uint32_t poweru32(uint32_t b, uint32_t p);

// my own lib function
uint8_t checksum(uint8_t *p, uint32_t count);


// if match, return 1
// if not match, return 0
uint8_t strmatch(const char *s1, const char *s2,  uint32_t count);


#endif 