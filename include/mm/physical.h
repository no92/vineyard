#pragma once

#include <init/multiboot2.h>

void mm_physical_init(multiboot2_t *multiboot);
void *mm_physical_alloc(void);
