#include <stddef.h>
#include <boot/multiboot.h>

multiboot2_tag_t *multiboot2_get_tag(multiboot2_t *multiboot, uint32_t type) {
	/* calculate where the tags start */
	uint32_t addr = (uint32_t) multiboot + sizeof(multiboot->total_size) + sizeof(multiboot->reserved);
	/* calculate the address of the last tag */
	uint32_t addr_limit = addr + multiboot->total_size;

	/* loop until we've looked through all tags or found the tag */
	while(addr < addr_limit) {
		multiboot2_tag_t *tag = (multiboot2_tag_t *) addr;

		/* if we're looking for this one, return it */
		if(tag->type == type) {
			return tag;
		}

		/* round the tag size up to the nearest 8-byte boundary */
		uint32_t size = (tag->size + 7) & ((uint32_t) ~0x7);
		addr += size;
	}

	return NULL;
}
