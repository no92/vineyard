#pragma once

#include <lock/spinlock.h>
#include <proc/thread.h>

typedef struct {
	spinlock_t *lock;
	list_t *queue;
	thread_t *holding;
} mutex_t;

mutex_t *mutex_create(void);
void mutex_remove(mutex_t *mutex);

void mutex_acquire(mutex_t *mutex);
void mutex_release(mutex_t *mutex);
