#pragma once

#include <stdint.h>

typedef int32_t ptrdiff_t;
typedef uint32_t size_t;

#define NULL ((void*)0)
#define offsetof(type, member) __builtin_offsetof(type, member)
