#include <_/vineyard.h>
#include <fs/initrd.h>
#include <mm/physical.h>
#include <mm/virtual.h>
#include <proc/elf32.h>

#include <assert.h>
#include <stdlib.h>
#include <string.h>

uintptr_t elf32_load(const char *path) {
	initrd_file_t *file = initrd_open(path);

	if(!file) {
		panic("process \"%s\" not found!", path);
	}

	elf32_header_t *header = (elf32_header_t *) malloc(initrd_file_size(file->size));
	uintptr_t file_start = ALIGN_UP((uintptr_t) file + sizeof(*file), 0x200);
	memcpy(header, (void *) file_start, initrd_file_size(file->size));

	assert(header->e_ident[0] == 0x7F && header->e_ident[1] == 'E' && header->e_ident[2] == 'L' && header->e_ident[3] == 'F');

	for(uintptr_t x = 0; x < (uint32_t) header->e_shentsize * header->e_shnum; x += header->e_shentsize) {
		elf32_shdr_t *shdr = (elf32_shdr_t *) ((uintptr_t) header + header->e_shoff + x);

		if(!shdr->sh_addr) {
			continue;
		}

		for(uintptr_t addr = shdr->sh_addr & 0xFFFFF000; addr < ALIGN_UP(shdr->sh_addr + shdr->sh_size, 0x1000); addr += 0x1000) {
			if(!(mm_virtual_get_physical(addr) & 0xFFFFF000)) {
				uintptr_t phys = (uintptr_t) (uintptr_t *) mm_physical_alloc();
				mm_virtual_map_page(addr & 0xFFFFF000, phys, PAGE_PRESENT | PAGE_WRITE | PAGE_USER);
			}
		}

		memcpy((void *) (shdr->sh_addr), (void *) ((uintptr_t) header + shdr->sh_offset), shdr->sh_size);
	}

	return header->e_entry;
}
