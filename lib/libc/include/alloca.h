#pragma once

#include <stddef.h>

void *alloca(size_t size);

#define alloca(size) __builtin_alloca(size)
