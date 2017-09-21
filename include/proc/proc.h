#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <sys/types.h>

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

typedef void (*proc_worker_t)(void *);

void proc_init(void);
void proc_create(const char *path, bool kernel, int argc, char **argv, int envc, char **envp);
list_node_t *proc_create_worker(proc_worker_t worker, void *arg);

__attribute__((pure)) proc_t *proc_get(void);

uintptr_t proc_exit(syscall_args_t *data);
uintptr_t sys_getpid(syscall_args_t *data);
