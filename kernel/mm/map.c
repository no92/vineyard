#include <init/multiboot2.h>
#include <mm/early.h>
#include <mm/map.h>
#include <util/list.h>

#include <stdio.h>

list_t mmap;

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
			list_node_t *node = mm_early_alloc(sizeof(list_node_t));
			map_entry_t *data = (map_entry_t *) mm_early_alloc(sizeof(map_entry_t));

			data->start = addr;
			data->end = addr + length - 1;
			data->type = entry->type;
			node->data = (void *) data;

			list_append(&mmap, node);
		}

		entry_addr += map->map.entry_size;
	}

	list_node_t *node = mmap.head;
	for(size_t i = 0; i < mmap.length; i++) {
		map_entry_t *c = node->data;

		printf("%#08x - %#08x: %s (%#x)\n", c->start, c->end, types[c->type], 1 + c->end - c->start);

		node = node->next;
	}
}
