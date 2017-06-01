#include <gfx/gfx.h>
#include <int/idt.h>
#include <mm/gdt.h>
#include <mm/map.h>
#include <mm/physical.h>
#include <mm/virtual.h>

#include <assert.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#define PAGE_PRESENT 		(1 << 0)
#define PAGE_WRITE	 		(1 << 1)
#define PAGE_USER	 		(1 << 2)
#define PAGE_WRITE_THROUGH	(1 << 3)
#define PAGE_NO_CACHE		(1 << 4)

uint32_t page_directory[1024] __attribute__((aligned(4096)));

NO_UBSAN static void mm_virtual_map_page(uintptr_t virt, uintptr_t phys, uintptr_t flags) {
	size_t index_dir = virt >> 22;
	size_t index_tab = (virt >> 12) & 0x03FF;

	if(!page_directory[index_dir]) {
		void *table = mm_physical_alloc();
		memset(table, 0x0, 0x400);
		page_directory[index_dir] = ((uintptr_t) table) | PAGE_PRESENT | PAGE_WRITE | flags;
	}

	uint32_t *page_table = (uint32_t *) (page_directory[index_dir] & 0xFFFFF000);

	page_table[index_tab] = (phys & 0xFFFFF000) | PAGE_PRESENT | PAGE_WRITE | flags;
}

extern uintptr_t _kernel_start;
extern uintptr_t _kernel_end;

void mm_virtual_init(multiboot2_t *multiboot) {
	(void) multiboot;
	memset(page_directory, 0x00, sizeof(page_directory));

	uintptr_t kernel_start = (uintptr_t) &_kernel_start;
	uintptr_t kernel_end = ALIGN_UP((uintptr_t) &_kernel_end, 0x1000);

	uintptr_t framebuffer_start = (uintptr_t) &gfx_framebuffer & 0xFFFFF000;
	uintptr_t framebuffer_end = framebuffer_start - 1 + gfx_framebuffer_size;
	framebuffer_end = ALIGN_UP(framebuffer_end, 0x1000);

	size_t framebuffer_pages = ALIGN_UP(gfx_framebuffer_size, 0x1000) >> 12;
	printf("framebuffer_size: %#x, framebuffer_pages: %#x\n", gfx_framebuffer_size, framebuffer_pages);

	for(uintptr_t i = 0; i < framebuffer_pages; i++) {
		mm_virtual_map_page((uintptr_t) gfx_framebuffer + (i << 12), (uintptr_t) gfx_framebuffer + (i << 12), 0);
	}

	for(uintptr_t i = kernel_start; i < kernel_end; i += 0x1000) {
		mm_virtual_map_page(i, i, 0);
	}

	printf("kernel: %#08x - %#08x\n", kernel_start, kernel_end);
	printf("framebuffer: %#08x - %#08x\n", framebuffer_start, framebuffer_end);

	asm volatile ("mov %0, %%cr3" : : "r" (page_directory) : "memory");
	asm volatile ("mov %cr0, %eax; or $0x80000000, %eax; mov %eax, %cr0");
}
