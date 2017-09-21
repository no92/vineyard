#include <assert.h>
#include <stdlib.h>

#include <lock/spinlock.h>

spinlock_t *spinlock_create(void) {
	spinlock_t *lock = malloc(sizeof(*lock));

	return lock;
}

void spinlock_remove(spinlock_t *lock) {
	free(lock);
}

void spinlock_acquire(spinlock_t *lock) {
	int interrupts;

	asm volatile ("pushf; pop %0" : "=r" (interrupts));
	interrupts &= 0x200;

	if(interrupts) asm volatile ("cli");

	for(;;) {

		if(__sync_bool_compare_and_swap(&lock->lock, 0, 1)) {
			lock->interrupts = interrupts;
			break;
		}

	}
	
	if(interrupts) asm volatile ("sti");
}

void spinlock_release(spinlock_t *lock) {
	assert(__sync_bool_compare_and_swap(&lock->lock, 1, 0));

	if(lock->interrupts) {
		asm volatile ("sti");
	}
}
