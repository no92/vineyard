#include <init/panic.h>
#include <mm/early.h>
#include <mm/map.h>

#include <stdio.h>
#include <string.h>

list_t *memmap;

static void mm_map_reserve(uintptr_t start, uintptr_t end, uint32_t type) {
	list_node_t *node = mm_early_alloc(sizeof(list_node_t));
	map_entry_t *data = (map_entry_t *) mm_early_alloc(sizeof(map_entry_t));

	data->start = start;
	data->end = end;
	data->type = type;
	node->data = (void *) data;

	list_append(memmap, node);
}

extern uintptr_t _kernel_end;

void mm_map_init(multiboot2_t *multiboot) {
	multiboot2_tag_t *map = multiboot2_get_tag(multiboot, MB2_TYPE_MMAP);

	uintptr_t end = (uintptr_t) map + map->size;
	uintptr_t entry_addr = (uintptr_t) map + sizeof(map->type) + sizeof(map->size) + sizeof(map->map.entry_size) + sizeof(map->map.entry_version);

	entry_addr = (uintptr_t) map + sizeof(map->type) + sizeof(map->size) + sizeof(map->map.entry_size) + sizeof(map->map.entry_version);

	memmap = mm_early_alloc(sizeof(*memmap));
	memset(memmap, 0, sizeof(*memmap));

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

	list_node_t *node = memmap->head;

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

	node = memmap->head;

	return;
}

list_t *mm_map_get(void) {
	return memmap;
}
