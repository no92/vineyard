#include <_/vineyard.h>
#include <fs/initrd.h>
#include <mm/physical.h>
#include <mm/virtual.h>
#include <proc/elf32.h>
#include <gfx/gfx.h>

#include <assert.h>
#include <stdio.h>
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

	elf32_program_header_t *phdr = (elf32_program_header_t *) ((uintptr_t) header + header->e_phoff);

	for(size_t i = 0; i < header->e_phnum; i++) {
		if(phdr->p_type != 1) {
			continue;
		}

		uintptr_t addr = phdr->p_vaddr & 0xFFFFF000;
		uintptr_t end = ALIGN_UP(phdr->p_vaddr + phdr->p_memsz, 0x1000);

		for(; addr < end; addr += 0x1000) {
			void *phys = mm_physical_alloc();
			mm_virtual_map_page(addr, (uintptr_t) phys, PAGE_PRESENT | PAGE_WRITE | PAGE_USER);
			assert(mm_virtual_get_physical(addr) == (uintptr_t) phys);
		}

		memcpy((void *) phdr->p_vaddr, (void *) ((uintptr_t) header + phdr->p_offset), phdr->p_filesz);
		memset((void *) (phdr->p_vaddr + phdr->p_filesz), 0x00, phdr->p_memsz - phdr->p_filesz);

		phdr = (elf32_program_header_t *) ((uintptr_t) phdr + header->e_phentsize);
	}

	return header->e_entry;
}
