#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <sys/types.h>

#include <int/handler.h>
#include <proc/syscall.h>
#include <util/list.h>

typedef struct {
	pid_t pid;
	size_t threads;
	size_t alive;
	const char *name;
	uintptr_t cs;
	uintptr_t ds;
	uintptr_t cr3;
	void *heap;
	bool kernel;
	list_t *thread_list;
} proc_t;

void proc_init(void);
void proc_create(const char *path, bool kernel);
void proc_switch(frame_t *frame);
__attribute__((pure)) proc_t *proc_get(void);
uintptr_t proc_exit(syscall_args_t *data);
uintptr_t sys_getpid(syscall_args_t *data);
