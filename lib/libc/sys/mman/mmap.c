#ifdef __vineyard__

#include <_/syscall.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <stddef.h>
#include <stdio.h>
#include <stdint.h>

void *mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset) {
	uintptr_t val = SYSCALL6(0x03, (uintptr_t) addr, length, (uintptr_t) prot, (uintptr_t) flags, (uintptr_t) fd, (uintptr_t) offset);

	return (void *) val;
}

#endif
