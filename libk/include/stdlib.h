#pragma once

#include <stddef.h>

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

int abs(int j);
long labs(long j);
long long llabs(long long j);

#ifdef __libk
	#define FALLTHROUGH	__attribute__((fallthrough));
#else
	#define FALLTHROUGH ((void)0);
#endif
