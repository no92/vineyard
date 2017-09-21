#pragma once

#include <stdbool.h>
#include <stdint.h>

typedef struct {
	volatile uintptr_t lock;
	bool interrupts;
} spinlock_t;

void spinlock_acquire(spinlock_t *lock);
void spinlock_release(spinlock_t *lock);
