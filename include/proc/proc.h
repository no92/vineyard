#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <sys/types.h>

#include <int/handler.h>
#include <proc/syscall.h>

typedef struct {
	pid_t pid;
	const char *name;
	uintptr_t esp;
	uintptr_t ebp;
	uintptr_t eip;
	uintptr_t cs;
	uintptr_t ds;
	uintptr_t stack;
	uintptr_t cr3;
	bool kernel;
	enum {
		PROC_RUNNING,
		PROC_SUSPENDED,
		PROC_DEAD,
	} state;
} proc_t;

void proc_init(void);
void proc_create(const char *path, bool kernel);
void proc_switch(frame_t *frame);
__attribute__((pure)) proc_t *proc_get(void);
void proc_exit(syscall_t *data);
