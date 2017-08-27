#pragma once

#include <init/multiboot2.h>
#include <util/list.h>

#include <_/vineyard.h>
#include <stddef.h>

#define MAP_TYPE_RESERVED			0
#define MAP_TYPE_USABLE				1
#define MAP_TYPE_ACPI_RECLAIMABLE	2
#define MAP_TYPE_ACPI_NVS			3
#define MAP_TYPE_UNUSABLE			4
#define MAP_TYPE_DISABLED			5

typedef struct {
	uintptr_t start;
	uintptr_t end;
	uint32_t type;
} map_entry_t;

NO_UBSAN void mm_map_init(multiboot2_t *multiboot);
__attribute__((pure)) list_t *mm_map_get(void);
