#pragma once

#include <stdint.h>

typedef struct {
	uintptr_t arg1;
	uintptr_t arg2;
	uintptr_t arg3;
	uintptr_t arg4;
	uintptr_t arg5;
	uintptr_t arg6;
} syscall_args_t;

typedef uintptr_t (*syscall_t)(syscall_args_t *);

#define SYSCALL_EXIT 0
#define SYSCALL_WRITE 1
#define SYSCALL_GETPID 2
#define SYSCALL_MMAP 3

void syscall_init(void);
