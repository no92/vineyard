#pragma once

#include <_/types.h>
#include <stdint.h>

typedef __size_t size_t;
typedef __ptrdiff_t ptrdiff_t;

typedef struct {
	long long __ll;
	long double __ld;
} max_align_t;

typedef int32_t wchar_t;

#define NULL (void*)0
#define offsetof(type, member) __builtin_offsetof(type, member)
