#pragma once

#include <stdbool.h>
#include <stdint.h>

typedef struct {
	volatile int lock;
	bool interrupts;
} spinlock_t;

spinlock_t *spinlock_create(void);
void spinlock_remove(spinlock_t *lock);

void spinlock_acquire(spinlock_t *lock);
void spinlock_release(spinlock_t *lock);
