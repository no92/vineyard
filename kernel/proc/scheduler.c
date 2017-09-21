#include <_/vineyard.h>
#include <mm/virtual.h>
#include <proc/proc.h>
#include <proc/tss.h>
#include <proc/scheduler.h>

#include <assert.h>
#include <stdlib.h>

/* scheduler queue */
static list_t *queue;

static bool first_switch = true;

/* currently running thread */
thread_t *current;

proc_t *proc_get(void) {
	if(current) {
		return (proc_t *) current->proc;
	}

	return NULL;
}

thread_t *thread_get(void) {
	return current;
}

void scheduler_queue_add(list_node_t *node) {
	list_append(queue, node);
}

void scheduler_queue_remove(list_node_t *node) {
	list_remove(queue, node);
}

static thread_t *scheduler_get_next(void) {
	list_node_t *node = queue->head;

	for(; node; node = node->next) {
		thread_t *t = node->data;

		if(t->state == THREAD_WAITING) {
			list_remove(queue, node);
			list_append(queue, node);
		} else if(t->state == THREAD_SUSPENDED || t->state == THREAD_DEAD) {
			return t;
		}
	}

	return NULL;
}

void scheduler_init() {
	queue = malloc(sizeof(*queue));
}

void scheduler_set_idle(thread_t *idle) {
	assert(!queue->head);

	current = idle;
}

void scheduler_tick(frame_t *frame) {
	if(!queue->head) {
		return;
	}

	thread_t *next = scheduler_get_next();
	proc_t *nextp = next->proc;
	proc_t *currentp = current->proc;

	assert(next != current);
	assert(current->state == THREAD_RUNNING || current->state == THREAD_DEAD);

	/* save registers of preempted thread */
	if(!first_switch) {
		current->esp = frame->esp;
		current->ebp = frame->ebp;
		current->eip = frame->eip;
		current->edi = frame->edi;
		current->esi = frame->esi;
		current->ebx = frame->ebx;
		current->edx = frame->edx;
		current->ecx = frame->ecx;
		current->eax = frame->eax;

		currentp->cs = frame->cs;
		currentp->ds = frame->ds;
	} else {
		first_switch = false;
	}

	/* pop the next process off the queue */
	list_node_t *node = queue->head;
	list_remove(queue, queue->head);

	if(current->state != THREAD_DEAD) {
		/* queue the preemted process */
		node->data = current;
		list_append(queue, node);

		/* mark it as suspended */
		if(current->state == THREAD_RUNNING) {
			current->state = THREAD_SUSPENDED;
		}
	} else {
		currentp->alive--;
		/* clean up the dead thread */
		free(container_of((void *) current, list_node_t, data));
		free(current);
	}

	if(!currentp->alive) {
		free(currentp->thread_list);
		free(currentp);
	}

	next->state = THREAD_RUNNING;

	/* restore the next process' registers */
	frame->eip = next->eip;
	frame->esp = next->esp;
	frame->ebp = next->ebp;
	frame->edi = next->edi;
	frame->esi = next->esi;
	frame->ebx = next->ebx;
	frame->edx = next->edx;
	frame->ecx = next->ecx;
	frame->eax = next->eax;
	frame->cs = nextp->cs;
	frame->ds = nextp->ds;
	frame->es = nextp->ds;
	frame->fs = nextp->ds;
	frame->gs = nextp->ds;
	frame->ss = nextp->ds;
	frame->eflags |= 0x200;

	if(currentp->pid != nextp->pid) {
		mm_virtual_switch(nextp->cr3);
	}

	current = next;

	tss_set_esp(next->kernel_stack);
}
