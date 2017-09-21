#include <assert.h>

#include <lock/spinlock.h>

void spinlock_acquire(spinlock_t *lock) {
	uintptr_t eflags;
	bool interrupts;

	asm volatile ("pushf; pop %%eax; mov %%eax, %0; popf" : "=r" (eflags) : : "%eax");
	interrupts = (eflags | 0x200) ? true : false;

	for(;;) {
		if(interrupts) asm volatile ("cli");

		if(__sync_bool_compare_and_swap(&lock->lock, 0, 1)) {
			lock->interrupts = interrupts;
			break;
		}

		if(interrupts) asm volatile ("sti");
	}
}

void spinlock_release(spinlock_t *lock) {
	assert(__sync_bool_compare_and_swap(&lock->lock, 1, 0));

	if(lock->interrupts) {
		asm volatile ("sti");
	}
}
