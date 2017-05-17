#include <init/multiboot2.h>
#include <mm/map.h>
#include <stdio.h>

static const char* types[] = {
	"Reserved",
	"Usable",
	"ACPI reclaimable",
	"ACPI NVS",
	"Unusable",
	"Disabled",
};

void mm_map_init(multiboot2_t *multiboot) {
	multiboot2_tag_t *map = multiboot2_get_tag(multiboot, MB2_TYPE_MMAP);

	uintptr_t end = (uintptr_t) map + map->size;

	uintptr_t entry_addr = (uintptr_t) map + sizeof(map->type) + sizeof(map->size) + sizeof(map->map.entry_size) + sizeof(map->map.entry_version);

	while(entry_addr < end) {
		multiboot2_map_entry_t *entry = (multiboot2_map_entry_t *) entry_addr;
		uint32_t addr = entry->addr & 0xFFFFFFFF;
		uint32_t length = entry->length & 0xFFFFFFFF;

		if(entry->length != 0) {
			printf("%#08x - %#08x: (%x) %s\n", addr, addr + length - 1, length, types[entry->type]);
		}

		entry_addr += map->map.entry_size;
	}
}
