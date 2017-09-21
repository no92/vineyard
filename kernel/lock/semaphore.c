#include <lock/semaphore.h>
#include <lock/spinlock.h>
#include <proc/thread.h>

#include <stdlib.h>

semaphore_t *semaphore_create(void) {
	return semaphore_create_init(0);
}

semaphore_t *semaphore_create_init(int initial) {
	semaphore_t *semaphore = malloc(sizeof(*semaphore));

	if(!semaphore) {
		return NULL;
	}

	semaphore->counter = initial;
	semaphore->lock = spinlock_create();
	semaphore->queue = malloc(sizeof(*semaphore->queue));

	return semaphore;
}

void semaphore_remove(semaphore_t *semaphore) {
	spinlock_remove(semaphore->lock);
	free(semaphore->queue);
	free(semaphore);
}

void semaphore_wait(semaphore_t *semaphore, int decrement) {
	spinlock_acquire(semaphore->lock);

	thread_t *t = thread_get();

	if(semaphore->counter >= 0) {
		semaphore->counter -= decrement;

		if(semaphore->counter < 0) {
			list_node_t *node = malloc(sizeof(*node));
			node->data = t;
			t->state = THREAD_WAITING;
			list_append(semaphore->queue, node);
			spinlock_release(semaphore->lock);

			thread_yield();
		} else {
			spinlock_release(semaphore->lock);
			return;
		}
	} else {
		list_node_t *node = malloc(sizeof(*node));
		node->data = t;
		t->state = THREAD_WAITING;
		list_append(semaphore->queue, node);
		spinlock_release(semaphore->lock);

		thread_yield();
	}
}

void semaphore_signal(semaphore_t *semaphore, int increment) {
	spinlock_acquire(semaphore->lock);

	semaphore->counter += increment;

	if(semaphore->counter >= 0 && semaphore->queue->head) {
		list_node_t *next_node = semaphore->queue->head;
		thread_t *next = next_node->data;
		next->state = THREAD_SUSPENDED;
		list_remove(semaphore->queue, next_node);
	}

	spinlock_release(semaphore->lock);
}
