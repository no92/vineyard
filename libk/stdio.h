#pragma once

#include <stdarg.h>
#include <stddef.h>

int puts(const char *str);

int printf(const char * restrict format, ...);

int snprintf(char * restrict s, size_t n, const char *restrict format, ...);

int vprintf(const char * restrict format, va_list arg);
int vsnprintf(char * restrict s, size_t n, const char * restrict format, va_list arg);
