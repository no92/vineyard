#include <_/vineyard.h>
#include <gfx/gfx.h>
#include <mm/physical.h>
#include <mm/virtual.h>

#include <assert.h>
#include <string.h>
#include <stdio.h>

#define PAGE_PRESENT 		(1 << 0)
#define PAGE_WRITE	 		(1 << 1)
#define PAGE_USER	 		(1 << 2)
#define PAGE_WRITE_THROUGH	(1 << 3)
#define PAGE_NO_CACHE		(1 << 4)

uint32_t *page_directory = (uint32_t *) 0xFFFFF000;
uint32_t *page_tables = (uint32_t *) 0xFFC00000;

static void mm_virtual_invlpg(uintptr_t addr) {
	asm volatile("invlpg (%0)" :: "r" (addr) : "memory");
}

static void mm_virtual_page_touch(uintptr_t page) {
	if(!(page_directory[page >> 22] & PAGE_PRESENT)) {
		page_directory[page >> 22] = (uintptr_t) (uintptr_t *) mm_physical_alloc() | PAGE_PRESENT | PAGE_WRITE;
		mm_virtual_invlpg((uintptr_t) &page_tables[page >> 12] & 0xFFFFF000);
		memset((void *) ((uintptr_t) &page_tables[page >> 12] & 0xFFFFF000), 0, 0x1000);
	}
}

void mm_virtual_map_page(uintptr_t virt, uintptr_t phys, uintptr_t flags) {
	assert(!(virt & 0x3FF));

	mm_virtual_page_touch(virt);
	page_tables[virt >> 12] = phys | flags;
	mm_virtual_invlpg(virt);
}

void mm_virtual_map(uintptr_t virt, size_t length, uintptr_t flags) {
	size_t n = ALIGN_UP(length, 0x1000) >> 12;

	for(size_t i = 0; i < n; i++) {
		mm_virtual_map_page(virt + (n << 12), (uintptr_t) (uintptr_t *) mm_physical_alloc(), flags);
	}
}

static void mm_virtual_unmap_page(uintptr_t virt) {
	size_t index_dir = virt >> 22;
	size_t index_tab = (virt >> 12) & 0x03FF;

	assert(page_directory[index_dir]);

	uint32_t *page_table = (uint32_t *) (page_directory[index_dir] & 0xFFFFF000);

	page_table[index_tab] = 0x00;

	mm_virtual_invlpg(virt);
}

void mm_virtual_unmap(uintptr_t virt, size_t length) {
	size_t n = ALIGN_UP(length, 0x1000);

	for(size_t i = 0; i < n; i++) {
		mm_virtual_unmap_page(virt + (n << 12));
	}
}

void mm_virtual_init(void) {
	for(size_t i = 0; i < gfx_framebuffer_size; i += 0x1000) {
		mm_virtual_map_page((uintptr_t) gfx_framebuffer + i, (uintptr_t) gfx_framebuffer + i, PAGE_PRESENT | PAGE_WRITE);
	}
}
