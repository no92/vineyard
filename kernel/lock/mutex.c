#include <lock/mutex.h>
#include <lock/spinlock.h>

#include <stdlib.h>

mutex_t *mutex_create(void) {
	mutex_t *mutex = malloc(sizeof(*mutex));

	if(!mutex) {
		return NULL;
	}

	mutex->lock = spinlock_create();
	mutex->queue = malloc(sizeof(*mutex->queue));

	return mutex;
}

void mutex_remove(mutex_t *mutex) {
	spinlock_remove(mutex->lock);
	free(mutex->queue);
	free(mutex);
}

void mutex_acquire(mutex_t *mutex) {
	spinlock_acquire(mutex->lock);

	thread_t *t = thread_get();

	if(mutex->holding) {
		list_node_t *node = malloc(sizeof(*node));
		node->data = t;
		t->state = THREAD_WAITING;
		list_append(mutex->queue, node);
		spinlock_release(mutex->lock);

		asm volatile ("int $0x81");
	} else {
		mutex->holding = t;
		spinlock_release(mutex->lock);
	}
}

void mutex_release(mutex_t *mutex) {
	spinlock_acquire(mutex->lock);

	mutex->holding = NULL;

	if(mutex->queue->head) {
		list_node_t *next_node = mutex->queue->head;
		thread_t *next = next_node->data;
		next->state = THREAD_SUSPENDED;
		list_remove(mutex->queue, next_node);
	}

	spinlock_release(mutex->lock);
}
