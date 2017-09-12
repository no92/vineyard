#pragma once

#include <stddef.h>
#include <sys/types.h>

#define MAP_ANONYMOUS	1 << 0
#define MAP_PRIVATE		1 << 1

#define PROT_EXEC		1 << 0
#define PROT_NONE		1 << 1
#define PROT_READ		1 << 2
#define PROT_WRITE		1 << 3

void *mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset);
int munmap(void *addr, size_t len);
