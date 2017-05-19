#include <mm/early.h>
#include <init/panic.h>

#include <stddef.h>
#include <stdint.h>

#define SIZE 16384

static uint8_t buf[SIZE];
static size_t off = 0;

void *mm_early_alloc(size_t size) {
	if(off + size > SIZE) {
		panic("out of early memory");
		return NULL;
	}

	void *ptr = &buf[off];
	off += size;

	return ptr;
}
