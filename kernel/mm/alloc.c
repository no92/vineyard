#include <assert.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <sys/mman.h>

#include <_/vineyard.h>
#include <mm/alloc.h>
#include <mm/physical.h>
#include <mm/virtual.h>
#include <proc/proc.h>

alloc_node_t *mm_alloc_areas;

void mm_alloc_init(void) {
	uintptr_t start = 0xD0000000;
	uintptr_t end = 0xEFFFFFFF;

	uintptr_t root = (uintptr_t) (uintptr_t *) mm_physical_alloc();

	mm_alloc_areas = (alloc_node_t *) start;

	mm_virtual_map_page(start, root, PAGE_PRESENT | PAGE_WRITE);

	mm_alloc_areas->next = 0;
	mm_alloc_areas->prev = 0;
	mm_alloc_areas->state = FREE;
	mm_alloc_areas->start = start + 0x1000;
	mm_alloc_areas->end = end;
}

static alloc_node_t *mm_alloc_find(alloc_node_t *root, size_t size) {
	for(alloc_node_t *node = root; node; node = node->next) {
    	if(node->state != FREE) {
			continue;
		}

		size_t node_size = node->end - node->start + 1;

		if(node_size < size) {
			continue;
		}

	    if((node_size - size) >= (0x1000 * 2)) {
			uintptr_t phys = (uintptr_t) (uintptr_t *) mm_physical_alloc();

			if(phys) {
				alloc_node_t *next = (alloc_node_t *) ((uintptr_t) node + size + 0x1000);
				mm_virtual_map_page((uintptr_t) next, phys, PAGE_PRESENT | PAGE_WRITE);

				next->start = (uintptr_t) node + size + 0x2000;
				next->end = node->end;
				next->state = FREE;
				next->prev = node;
				next->next = node->next;

				node->end = (uintptr_t) next - 1;
				node->next = next;

				if(next->next) {
					next->next->prev = next;
				}
			}
		}

		node->state = RESERVED;

		return node;
	}

	return 0;
}

A("bitwise operator in conditional")
static bool mm_alloc_free_area(uintptr_t virt, size_t length) {
	assert(!(length & 0x3FF));

	for(size_t i = 0; i < length; i += 0x1000) {
		mm_physical_free(mm_virtual_unmap_page(virt));
	}

	return true;
}

A("bitwise operator in conditional")
static void mm_alloc_area(uintptr_t addr, size_t length, uint16_t flags) {
	assert(!(length & 0x3FF));

	for(size_t i = 0; i < length; i += 0x1000) {
		uintptr_t phys = (uintptr_t) (uintptr_t *) mm_physical_alloc();

		mm_virtual_map_page(addr + i, phys, flags);
	}
}

A("bitwise operator in conditional")
static void mm_alloc_free(void *ptr) {
	alloc_node_t *node = (alloc_node_t *) ((uintptr_t) ptr - 0x1000);
	size_t size = node->end - node->start + 1;

	if(node->state == ALLOCATED) {
		mm_alloc_free_area(node->start, size);
	}

	node->state = FREE;

	alloc_node_t *next = node->next;

	if(next && next->state == FREE) {
		node->next = next->next;

		if(next->next) {
			next->next->prev = node;
		}

		node->end = next->end;

		mm_physical_free(mm_virtual_unmap_page((uintptr_t) next));
	}

	alloc_node_t *prev = node->prev;

	if(prev && prev->state == FREE) {
		prev->next = node->next;

		if(node->next) {
			node->next->prev = prev;
		}

		prev->end = node->end;

		mm_physical_free(mm_virtual_unmap_page((uintptr_t) node));
	}
}

void mm_alloc_reserve(uintptr_t addr, size_t s, uint16_t flags) {
	mm_alloc_area(addr, s, flags);
}

void *mm_alloc_proc(alloc_node_t *root, size_t s, uint16_t flags, bool allocate) {
	size_t size = ALIGN_UP(s, 0x1000);
	alloc_node_t *node = mm_alloc_find(root, size);

	if(!node) {
		panic("[alloc]	out of kernel heap");
		return NULL;
	}

	node->state = RESERVED;

	if(allocate) {
		node->state = ALLOCATED;
		node->flags = flags;

		mm_alloc_area(node->start, size, flags);
	}

	return (void *) ((uintptr_t) node + 0x1000);
}

void *mm_alloc(size_t s, uint16_t flags, bool allocate) {
	return mm_alloc_proc(mm_alloc_areas, s, flags, allocate);
}

uintptr_t sys_mmap(syscall_args_t *data) {
	void *ret = mmap((void *) data->arg1, data->arg2, (int) data->arg3, (int) data->arg4, (int) data->arg5, (off_t) data->arg6);

	return (uintptr_t) ret;
}

A("bitwise operator in conditional")
void *mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset) {
	(void) addr;
	(void) prot;
	(void) flags;
	(void) fd;
	(void) offset;

	proc_t *proc = proc_get();
	alloc_node_t *root = (proc) ? proc->heap : mm_alloc_areas;
	uint16_t f = PAGE_PRESENT | PAGE_WRITE;

	if(proc && !proc->kernel) {
		f = f | PAGE_USER;
	}

	void *ptr = mm_alloc_proc(root, length, f, true);
	memset(ptr, 0x00, length);

	return ptr;
}

uintptr_t sys_munmap(syscall_args_t *data) {
	int ret = munmap((void *) data->arg1, data->arg2);

	return (uintptr_t) ret;
}

A("bitwise operator in conditional")
int munmap(void *addr, size_t len) {
	if(!addr || ((uintptr_t) addr & 0x3FF) || !len) {
		errno = EINVAL;
		return -1;
	}

	mm_alloc_free(addr);

	return 0;
}
