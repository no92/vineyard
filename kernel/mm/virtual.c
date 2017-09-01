#include <_/vineyard.h>
#include <gfx/gfx.h>
#include <int/handler.h>
#include <mm/physical.h>
#include <mm/virtual.h>
#include <util/trace.h>

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

A("bitwise operator in conditional")
static void mm_virtual_page_touch(uintptr_t page) {
	if(!(page_directory[page >> 22] & PAGE_PRESENT)) {
		page_directory[page >> 22] = (uintptr_t) (uintptr_t *) mm_physical_alloc() | PAGE_PRESENT | PAGE_WRITE;
		mm_virtual_invlpg((uintptr_t) &page_tables[page >> 12] & 0xFFFFF000);
		memset((void *) ((uintptr_t) &page_tables[page >> 12] & 0xFFFFF000), 0, 0x1000);
	}
}

A("bitwise operator in conditional")
uintptr_t mm_virtual_map_page(uintptr_t virt, uintptr_t phys, uintptr_t flags) {
	assert(!(virt & 0x3FF));

	mm_virtual_page_touch(virt);
	page_tables[virt >> 12] = phys | flags;
	mm_virtual_invlpg(virt);

	return virt;
}

void mm_virtual_map(uintptr_t virt, size_t length, uintptr_t flags) {
	size_t n = ALIGN_UP(length, 0x1000) >> 12;

	for(size_t i = 0; i < n; i++) {
		mm_virtual_map_page(virt + (n << 12), (uintptr_t) (uintptr_t *) mm_physical_alloc(), flags);
	}
}

uintptr_t mm_virtual_unmap_page(uintptr_t virt) {
	uintptr_t phys = mm_virtual_get_physical(virt);

	page_tables[virt >> 12] = 0x00;

	mm_virtual_invlpg(virt);

	return phys;
}

void mm_virtual_unmap(uintptr_t virt, size_t length) {
	size_t n = ALIGN_UP(length, 0x1000);

	for(size_t i = 0; i < n; i++) {
		mm_physical_free(mm_virtual_unmap_page(virt + (n << 12)));
	}
}

uintptr_t mm_virtual_get_physical(uintptr_t virt) {
	return page_tables[virt >> 12] & 0xFFFFF000;
}

uint16_t mm_virtual_get_flags(uintptr_t virt) {
	return page_tables[virt >> 12] & 0xFFF;
}

static void mm_virtual_page_fault(frame_t *frame) {
	uint32_t cr2;
	asm volatile ("mov %%cr2, %0" : "=r" (cr2));

	const char *present = (frame->error & 0x01) ? "present" : "non-present";
	const char *access = (frame->error & 0x02) ? "write to" : "read from";
	const char *mode = (frame->error & 0x04) ? "user" : "kernel";

	printf("Page Fault at %#08x <%s>: %s address %#08x by %s to %s page\n", frame->eip, trace_lookup_addr(frame->eip), access, cr2, mode, present);
}

void mm_virtual_init(void) {
	handler_set(0x0E, mm_virtual_page_fault);

	for(size_t i = 0; i < gfx_framebuffer_size; i += 0x1000) {
		mm_virtual_map_page((uintptr_t) gfx_framebuffer + i, (uintptr_t) gfx_framebuffer + i, PAGE_PRESENT | PAGE_WRITE);
	}
}
