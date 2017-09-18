#pragma once

#include <proc/syscall.h>

#include <stdbool.h>
#include <stddef.h>

enum heap_state {
	FREE,
	RESERVED,
	ALLOCATED,
};

typedef struct node {
	struct node *next;
	struct node *prev;
	uintptr_t start;
	uintptr_t end;
	uint16_t flags;
	enum heap_state state;
} alloc_node_t;

extern alloc_node_t *mm_alloc_areas;

void mm_alloc_init(void);
void mm_alloc_info(void);

void *mm_alloc_proc(alloc_node_t *root, size_t s, uint16_t flags, bool allocate);
void *mm_alloc(size_t s, uint16_t flags, bool allocate);
void mm_alloc_reserve(uintptr_t addr, size_t s, uint16_t flags);

uintptr_t sys_mmap(syscall_args_t *data);
uintptr_t sys_munmap(syscall_args_t *data);
