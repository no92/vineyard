#include <init/panic.h>
#include <mm/early.h>
#include <mm/map.h>
#include <mm/physical.h>
#include <util/bitmap.h>

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define BITS_PAGE_OFF 12

static bitmap_t *pages;
/* size of the bitmap in bytes */
static size_t n;
/* the limit of usable memory */
static uintptr_t limit;

static void mm_physical_mark_free(uintptr_t addr) {
	bitmap_unset(pages, addr >> 12);
}

static void mm_physical_mark_reserved(uintptr_t addr) {
	bitmap_set(pages, addr >> 12);
}

void mm_physical_init(multiboot2_t *multiboot) {
	multiboot2_tag_t *meminfo = multiboot2_get_tag(multiboot, MB2_TYPE_MEMINFO);

	/* the meminfo tag gives us the size in KiB; log2(1024) = 10 */
	limit = (meminfo->mem.upper + 1024) << 10;

	list_t *map = mm_map_get();
	list_node_t *node = map->head;

	n = limit >> (BITS_PAGE_OFF + 3);
	pages = (bitmap_t *) mm_early_alloc(n);
	memset(pages, 0xFF, n);

	for(size_t i = 0; i < map->length; i++) {
		map_entry_t *c = node->data;
		uintptr_t start = c->start;
		uintptr_t end = c->end;
		size_t length = (1 + end - start) >> BITS_PAGE_OFF;

		if(c->type != MAP_TYPE_USABLE) {
			node = node->next;
			continue;
		}

		for(size_t j = 0; j < length; j++) {
			mm_physical_mark_free(start + (j << BITS_PAGE_OFF));
		}

		node = node->next;
	}

	mm_physical_mark_reserved(0);
}

void *mm_physical_alloc(void) {
	ssize_t page = bitmap_first_unset(pages);

	if(page == -1) {
		panic("out of physical memory");
	}

	bitmap_set(pages, (size_t) page);

	return (void *) (page << 12);
}
