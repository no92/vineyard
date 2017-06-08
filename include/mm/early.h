#pragma once

#include <stddef.h>

void mm_early_config(uintptr_t ptr, size_t len);
void *mm_early_alloc(size_t size);
