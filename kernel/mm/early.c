#include <mm/early.h>
#include <init/panic.h>

#define POOL_SIZE	0x100000

static uint8_t pool[POOL_SIZE];
static size_t off;

void *mm_early_alloc(size_t s) {
	if(off + s > POOL_SIZE) {
		panic("out of early memory");
	}

	void *ret = &pool[off];

	off += s;

	return ret;
}
