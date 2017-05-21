#include <init/multiboot2.h>
#include <init/panic.h>
#include <mm/early.h>
#include <mm/map.h>
#include <util/list.h>

#include <stdio.h>

#define ALIGN_UP(addr, alignment)	((addr + (alignment - 1)) & ~(alignment - 1))

list_t mmap;

static const char* types[] = {
	[MAP_TYPE_RESERVED] = "Reserved",
	[MAP_TYPE_USABLE] = "Usable",
	[MAP_TYPE_ACPI_RECLAIMABLE] = "ACPI reclaimable",
	[MAP_TYPE_ACPI_NVS] = "ACPI NVS",
	[MAP_TYPE_UNUSABLE] = "Unusable",
	[MAP_TYPE_DISABLED] = "Disabled",
};

static void mm_map_reserve(uintptr_t start, uintptr_t end, uint32_t type) {
	list_node_t *node = mm_early_alloc(sizeof(list_node_t));
	map_entry_t *data = (map_entry_t *) mm_early_alloc(sizeof(map_entry_t));

	data->start = start;
	data->end = end;
	data->type = type;
	node->data = (void *) data;

	list_append(&mmap, node);
}

void mm_map_init(multiboot2_t *multiboot) {
	multiboot2_tag_t *map = multiboot2_get_tag(multiboot, MB2_TYPE_MMAP);

	uintptr_t end = (uintptr_t) map + map->size;
	uintptr_t entry_addr = (uintptr_t) map + sizeof(map->type) + sizeof(map->size) + sizeof(map->map.entry_size) + sizeof(map->map.entry_version);

	while(entry_addr < end) {
		multiboot2_map_entry_t *entry = (multiboot2_map_entry_t *) entry_addr;
		uint32_t start = entry->addr & 0xFFFFF000;
		uint32_t limit = (uint32_t) (entry->addr + entry->length - 1);
		limit = ALIGN_UP(limit, 4096U) - 1;

		if(entry->length != 0) {
			mm_map_reserve(start, limit, entry->type);
		}

		entry_addr += map->map.entry_size;
	}

	list_node_t *node = mmap.head;

	while(node->next) {
		map_entry_t *c = node->data;
		map_entry_t *n = node->next->data;

		if(c->end > n->start) {
			/* overlap */
			if(c->type == MAP_TYPE_USABLE && n->type != MAP_TYPE_USABLE) {
				c->end = n->start - 1;
			} else if(c->type != MAP_TYPE_USABLE && n->type == MAP_TYPE_USABLE) {
				n->start = c->end + 1;
			} else {
				panic("unimplemented overlap!");
			}
		}

		node = node->next;
	}

	node = mmap.head;

	while(node) {
		map_entry_t *c = node->data;
		size_t length = (1 + c->end - c->start) >> 12;

		printf("%#08x - %#08x (%#06x pages) %s\n", c->start, c->end, length, types[c->type]);

		node = node->next;
	}
}

list_t *mm_map_get(void) {
	return &mmap;
}
