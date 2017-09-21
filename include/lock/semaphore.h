#pragma once

#include <lock/spinlock.h>
#include <util/list.h>

#include <stdint.h>

typedef struct {
	spinlock_t *lock;
	int counter;
	list_t *queue;
} semaphore_t;

semaphore_t *semaphore_create(void);
semaphore_t *semaphore_create_init(int initial);
void semaphore_remove(semaphore_t *semaphore);
void semaphore_wait(semaphore_t *semaphore, int decrement);
void semaphore_signal(semaphore_t *semaphore, int increment);
