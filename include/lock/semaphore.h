#pragma once

#include <lock/spinlock.h>
#include <stdint.h>

typedef struct {
	spinlock_t *lock;
	const char *name;
	uint32_t counter;
} semaphore_t;
