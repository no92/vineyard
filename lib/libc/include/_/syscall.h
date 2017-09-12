#pragma once

#include <_/types.h>

#define SYSCALL0(num) ({ __uintptr_t res; asm volatile ("int $0x80" : "=a" (res) : "0" (num)); res; })
#define SYSCALL1(num, p1) ({ __uintptr_t res; asm volatile ("int $0x80" : "=a" (res) : "0" (num), "b" (p1)); res; })
#define SYSCALL2(num, p1, p2) ({ __uintptr_t res; asm volatile ("int $0x80" : "=a" (res) : "0" (num), "b" (p1), "c" (p2)); res; })
#define SYSCALL3(num, p1, p2, p3) ({ __uintptr_t res; asm volatile ("int $0x80" : "=a" (res) : "0" (num), "b" (p1), "c" (p2), "d" (p3)); res; })
