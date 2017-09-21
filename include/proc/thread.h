#pragma once

#include <stdint.h>

typedef uintptr_t tid_t;

#include <proc/proc.h>

typedef struct {
	uintptr_t kernel_stack;
	tid_t tid;
	proc_t *proc;
	uintptr_t esp;
	uintptr_t ebp;
	uintptr_t eip;
	uintptr_t edi;
	uintptr_t esi;
	uintptr_t ebx;
	uintptr_t edx;
	uintptr_t ecx;
	uintptr_t eax;

	enum {
		THREAD_RUNNING,
		THREAD_SUSPENDED,
		THREAD_WAITING,
		THREAD_DEAD,
	} state;
} thread_t;

thread_t *thread_create(proc_t *proc, uintptr_t start);
__attribute__((pure)) thread_t *thread_get(void);
