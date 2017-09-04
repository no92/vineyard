#pragma once

#include <stdbool.h>
#include <stddef.h>

void mm_alloc_init(void);
void mm_alloc_info(void);

void *mm_alloc(size_t s, uint16_t flags, bool allocate);
void mm_alloc_reserve(uintptr_t addr, size_t s, uint16_t flags);
