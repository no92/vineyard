#pragma once

#include <stdint.h>

typedef struct {
	uintptr_t arg1;
	uintptr_t arg2;
	uintptr_t arg3;
	uintptr_t arg4;
	uintptr_t arg5;
	uintptr_t arg6;
} syscall_t;

void syscall_init(void);
