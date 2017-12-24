#pragma once

#include <_/types.h>
#include <stdarg.h>
#include <stddef.h>

typedef __FILE FILE;

extern FILE * const stdin;
extern FILE * const stdout;
extern FILE * const stderr;

#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2

#define EOF (-1)

int puts(const char *str);

__attribute__((format(printf, 1, 2))) int printf(const char * restrict format, ...);

__attribute__((format(printf, 3, 4))) int snprintf(char * restrict s, size_t n, const char *restrict format, ...);

__attribute__((format(printf, 1, 0))) int vprintf(const char * restrict format, va_list arg);
__attribute__((format(printf, 3, 0))) int vsnprintf(char * restrict s, size_t n, const char * restrict format, va_list arg);

int getchar(void);
