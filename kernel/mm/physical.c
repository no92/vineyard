#include <init/panic.h>
#include <mm/early.h>
#include <mm/map.h>
#include <mm/physical.h>

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define MASK_ADDR	0xFFFFF000
#define BITS_INDEX	12
#define MASK_INDEX	0x0FFF
#define BITS_ENTRY	3
#define MASK_ENTRY	0x07

static uint8_t *pages;
/* size of the bitmap in bytes */
static size_t n;
static uintptr_t limit;

static void mm_physical_free(uintptr_t addr) {
	size_t page = addr >> BITS_INDEX;
	size_t index = page >> BITS_ENTRY;
	size_t offset = page & MASK_ENTRY;

	pages[index] &= (uint8_t) ~(1 << offset);
}

static void mm_physical_reserve(uintptr_t addr) {
	size_t page = addr >> BITS_INDEX;
	size_t index = page >> BITS_ENTRY;
	size_t offset = page & MASK_ENTRY;

	pages[index] |= (uint8_t) (1 << offset);
}

void mm_physical_init(multiboot2_t *multiboot) {
	multiboot2_tag_t *meminfo = multiboot2_get_tag(multiboot, MB2_TYPE_MEMINFO);
	/* the meminfo tag gives us the size in KiB; log2(1024) = 10 */
	limit = (meminfo->mem.upper + 1024) << 10;

	list_t *map = mm_map_get();
	list_node_t *node = map->head;

	n = limit >> (BITS_INDEX + BITS_ENTRY);
	pages = (uint8_t *) mm_early_alloc(n);
	memset(pages, 0xFF, n);

	for(size_t i = 0; i < map->length; i++) {
		map_entry_t *c = node->data;
		uintptr_t start = c->start;
		uintptr_t end = c->end;
		size_t length = (1 + end - start) >> BITS_INDEX;

		if(c->type != MAP_TYPE_USABLE) {
			node = node->next;
			continue;
		}

		for(size_t j = 0; j < length; j++) {
			mm_physical_free(start + (j << BITS_INDEX));
		}

		node = node->next;
	}

	mm_physical_reserve(0);
}

void *mm_physical_alloc(void) {
	for(size_t i = 0; i < n; i++) {
		if(pages[i] == 0xFF) {
			continue;
		}

		size_t first_unset_bit = (size_t) (__builtin_ctz(~(uint32_t) pages[i]));

		if(first_unset_bit > 7) {
			continue;
		}

		printf("index: %u, first unset bit: %u\n", i, first_unset_bit);

		uintptr_t address = ((i << BITS_ENTRY) | first_unset_bit) << BITS_INDEX;

		mm_physical_reserve(address);

		return (void *) address;
	}

	panic("out of physical memory");
}
