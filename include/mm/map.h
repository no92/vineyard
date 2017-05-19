#pragma once

#include <init/multiboot2.h>

#include <stddef.h>

typedef struct {
	uintptr_t start;
	uintptr_t end;
	uint32_t type;
} map_entry_t;

__attribute__((no_sanitize_undefined)) void mm_map_init(multiboot2_t *multiboot);
