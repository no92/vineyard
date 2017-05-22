#pragma once

#include <stdarg.h>
#include <stddef.h>

int puts(const char *str);

__attribute__((format(printf, 1, 2))) int printf(const char * restrict format, ...);

__attribute__((format(printf, 3, 4))) int snprintf(char * restrict s, size_t n, const char *restrict format, ...);

__attribute__((format(printf, 1, 0))) int vprintf(const char * restrict format, va_list arg);
__attribute__((format(printf, 3, 0))) int vsnprintf(char * restrict s, size_t n, const char * restrict format, va_list arg);
