#pragma once

#include <stdint.h>

#ifdef __i386__
	typedef int32_t ptrdiff_t;
	typedef uint32_t size_t;
#elif __x86_64__
	typedef int64_t ptrdiff_t;
	typedef uint64_t size_t;
#else
	#error Building for an unsupported arch. How did you do that?
#endif

typedef struct {
	long long __ll;
	long double __ld;
} max_align_t;

typedef int32_t wchar_t;

#define NULL ((void*)0)
#define offsetof(type, member) __builtin_offsetof(type, member)
