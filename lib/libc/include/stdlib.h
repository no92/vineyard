#pragma once

#include <stddef.h>

#ifdef __libk
#include <mm/malloc.h>
#endif

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

#ifndef min
	#define min(a, b) __extension__({__typeof__(a) _a = (a); __typeof__(b) _b = (b); _a < _b ? _a : _b; })
#endif

#ifndef max
	#define max(a, b) __extension__({__typeof__(a) _a = (a); __typeof__(b) _b = (b); _a > _b ? _a : _b; })
#endif

void exit(int status);

int abs(int j);
long labs(long j);
long long llabs(long long j);

#ifdef __libk
	#define FALLTHROUGH	__attribute__((fallthrough))
#else
	#define FALLTHROUGH ((void)0);
#endif
