#include <mm/early.h>
#include <init/panic.h>

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

static uintptr_t start;
static size_t length;
static size_t off;

void mm_early_config(uintptr_t ptr, size_t len) {
	start = ptr;
	length = len;
}

void *mm_early_alloc(size_t size) {
	if(off + size > length) {
		panic("out of early memory");
		return NULL;
	}

	void *ptr = (void *) (start + off);
	off += size;

	return ptr;
}
