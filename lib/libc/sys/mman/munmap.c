#ifdef __vineyard__

#include <_/syscall.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <stddef.h>
#include <stdio.h>
#include <stdint.h>

int munmap(void *addr, size_t length) {
	uintptr_t val = SYSCALL2(0x04, (uintptr_t) addr, length);

	return (int) val;
}

#endif
