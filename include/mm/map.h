#pragma once

#include <init/multiboot2.h>

__attribute__((no_sanitize_undefined)) void mm_map_init(multiboot2_t *multiboot);
