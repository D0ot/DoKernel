#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>
#include "common.h"
#include "terminal.h"


int sprintf(char *str, const char* format, ...)
{
    int ret;
    va_list ap;
    va_start(ap, format);
    ret = v_sprintf(str, format, ap);
    va_end(ap);
    return ret;
}


int v_sprintf(char *str, const char* format, va_list ap)
{
    
    char *for_ret = str;

    char *temp_str;

    char buf[15];
    while(*format)
    {
        char ch = *format;
        switch (ch)
        {
        case '%' :

            switch (*(format + 1))
            {
            case 'd': // integer
                itoa(va_arg(ap, int), buf, 10);
                strcpy(str, buf);
                format += 2;
                str += strlen(buf); 
                break;

            case 'x': // integer, hex form
                utoa(va_arg(ap, unsigned), buf, 16);
                strcpy(str, buf);
                format += 2;
                str += strlen(buf); 
                break;

            case 's':
                temp_str = va_arg(ap, char*);
                strcpy(str, temp_str);
                format += 2;
                str += strlen(temp_str);
                break;

            case '%': // single '%'
                *str++ = '%';
                format += 2;
                break;

            default:
                *str++ = ch;
                format++;
                break;
            }

            break;
        default:
            *str++ = ch;
            format++;
            break;
        }
    }

    *(str) = '\0';
    return str - for_ret;

}

int printf(const char* format, ...)
{
    int ret;
    va_list ap;
    va_start(ap, format);
    ret = v_printf(format, ap);
    va_end(ap);
    return ret;

}

int v_printf(const char* format, va_list ap)
{
    char buf[256];
    int ret = v_sprintf(buf, format, ap);
    char *iter = buf;
    while(*iter)
    {
        putchar(*iter++);
    }
    return ret;
}


// inline function to swap two numbers
inline void swap(char *x, char *y) {
	char t = *x; *x = *y; *y = t;
}

// function to reverse buffer[i..j]
char* reverse(char *buffer, size_t i, size_t j)
{
	while (i < j)
		swap(&buffer[i++], &buffer[j--]);

	return buffer;
}

int abs(int value)
{
    return value > 0 ? value : -value;
}

// Iterative function to implement itoa() function in C
char* itoa(int value, char* buffer, int base)
{
	// invalid input
	if (base < 2 || base > 32)
		return buffer;

	// consider absolute value of number
	int n = abs(value);

	size_t i = 0;
	while (n)
	{
		int r = n % base;

		if (r >= 10) 
			buffer[i++] = 65 + (r - 10);
		else
			buffer[i++] = 48 + r;

		n = n / base;
	}

	// if number is 0
	if (i == 0)
		buffer[i++] = '0';

	// If base is 10 and value is negative, the resulting string 
	// is preceded with a minus sign (-)
	// With any other base, value is always considered unsigned
	if (value < 0 && base == 10)
		buffer[i++] = '-';

	buffer[i] = '\0'; // null terminate string

	// reverse the string and return it
	return reverse(buffer, 0, i - 1);
}

char *utoa(unsigned value, char* buffer, unsigned base)
{
    if(base < 2 || base > 32)
    {
        return buffer;
    }

    unsigned n = value;
    size_t i = 0;
    while(n)
    {
        unsigned r = n % base;
        if(r >= 10)
        {
            buffer[i++] = 65 + (r - 10);
        }
        else
        {
            buffer[i++] = 48 + r;
        }
        n = n / base;
    }
    
    if(i == 0)
        buffer[i++] = '0';
    
    buffer[i] = '\0';
    return reverse(buffer, 0, i - 1);
}


size_t strlen(const char *str)
{
    size_t len = 0;

    // count how many non-null char in str
    while(*str++)
    {
        ++len;
    }
    return len;
}

char *strcpy(char *dest, const char *src)
{
    char *ret = dest;
    while(*src)
    {
        *dest++ = *src++;
    }
    *dest = '\0';
    return ret; 
}

void *memset(void *ptr, char value, size_t num)
{
    char *iter = ptr;
    for(size_t i = 0; i < num; ++i)
    {
        iter[i] = value;
    }
    return ptr;
}


void *memcpy(void *dest, const void *src, size_t num)
{
    char *dest_iter = dest;
    const char *src_iter = src;
    for(size_t i = 0; i < num; ++i)
    {
        dest_iter[i] = src_iter[i];
    }
    return dest;
}

void *memmove(void *dest, const void *src, size_t num)
{
    return memcpy(dest, src, num);
}

