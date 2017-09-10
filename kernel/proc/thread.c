#include <mm/alloc.h>
#include <mm/virtual.h>
#include <proc/thread.h>

#include <stdlib.h>

thread_t *thread_create(proc_t *proc, uintptr_t start) {
	thread_t *thread = calloc(1, sizeof(thread_t));

	thread->tid = proc->threads++;
	thread->proc = proc;
	thread->eip = start;
	thread->esp = ((uintptr_t) (uintptr_t *) mm_alloc_proc(proc->heap, 0x1000, PAGE_PRESENT | PAGE_WRITE | PAGE_USER, true)) + 0x1000 - 4;
	thread->ebp = thread->esp;
	thread->state = THREAD_SUSPENDED;

	proc->alive++;

	return thread;
}
