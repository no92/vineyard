#include <_/vineyard.h>
#include <fs/initrd.h>
#include <mm/alloc.h>
#include <mm/virtual.h>
#include <util/list.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static list_t *files;
static uintptr_t initrd_start;
static size_t initrd_size;

__attribute__((unused)) static uint32_t initrd_file_size(const char *in) {
	uint32_t size = 0;
	uint32_t count = 1;

	for(uint32_t j = 11; j > 0; j--, count *= 8) {
		size += ((uint32_t) (in[j - 1] - '0') * count);
	}

	return size;
}

void initrd_init(multiboot2_t *multiboot) {
	multiboot2_tag_t *tag = multiboot2_get_tag(multiboot, MB2_TYPE_MODULE);
	files = malloc(sizeof(*files));

	initrd_start = tag->module.start;
	initrd_size = tag->module.end - initrd_start;

	void *area = mm_alloc(ALIGN_UP(initrd_size, 0x1000), 0, false);

	for(size_t i = 0; i < initrd_size; i += 0x1000) {
		mm_virtual_map_page((uintptr_t) area + i, initrd_start + i, PAGE_PRESENT | PAGE_WRITE);
	}

	initrd_start = (uintptr_t) area;

	for(initrd_file_t *file = (initrd_file_t *) area; strcmp(file->filename, ""); file = (initrd_file_t *) ALIGN_UP((uintptr_t) file + 0x200 + initrd_file_size(file->size), 0x200)) {
		list_node_t *node = (list_node_t *) malloc(sizeof(list_node_t));
		node->data = file;

		list_append(files, node);
	}
}