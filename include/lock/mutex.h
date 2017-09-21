#pragma once

#include <lock/spinlock.h>

typedef struct {
	spinlock_t *lock;
	const char *name;
} mutex_t;
