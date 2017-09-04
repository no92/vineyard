#pragma once

#include <init/multiboot2.h>

#include <stddef.h>

#define PAGE_PRESENT 		(1 << 0)
#define PAGE_WRITE	 		(1 << 1)
#define PAGE_USER	 		(1 << 2)
#define PAGE_WRITE_THROUGH	(1 << 3)
#define PAGE_NO_CACHE		(1 << 4)

void mm_virtual_init(void);

uintptr_t mm_virtual_map_page(uintptr_t virt, uintptr_t phys, uintptr_t flags);
uintptr_t mm_virtual_unmap_page(uintptr_t virt);

void mm_virtual_map(uintptr_t virt, size_t length, uintptr_t flags);
void mm_virtual_unmap(uintptr_t virt, size_t length);

uintptr_t mm_virtual_get_physical(uintptr_t virt);
uint16_t mm_virtual_get_flags(uintptr_t virt);

void mm_virtual_switch(uintptr_t cr3);
