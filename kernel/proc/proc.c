#include <_/vineyard.h>
#include <fs/initrd.h>
#include <mm/alloc.h>
#include <mm/physical.h>
#include <mm/virtual.h>
#include <proc/elf32.h>
#include <proc/proc.h>
#include <proc/thread.h>
#include <init/panic.h>
#include <util/list.h>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* pid counter */
static pid_t pid = 1;
/* scheduler queue */
static list_t *queue;
/* currently running thread */
static thread_t *current;
/* holds the physical address of the kernel PD */
static uintptr_t kernel_pd;

extern uintptr_t stack_top;

static noreturn void proc_idle(void) {
	while(1) asm volatile ("hlt");
}

proc_t *proc_get(void) {
	if(current) {
		return (proc_t *) current->proc;
	}

	return NULL;
}

void proc_init(void) {
	queue = malloc(sizeof(*queue));

	/* set up the idle process */
	proc_t *proc = malloc(sizeof(proc_t));
	proc->pid = pid++;
	proc->heap = mm_alloc_areas;
	proc->thread_list = malloc(sizeof(list_t));

	thread_t *thread = thread_create(proc, (uintptr_t) proc_idle);
	list_node_t *tnode = malloc(sizeof(list_node_t));
	thread->esp = 0;
	thread->ebp = 0;
	thread->eip = (uintptr_t) proc_idle;
	thread->state = THREAD_RUNNING;
	tnode->data = thread;
	list_append(proc->thread_list, tnode);

	proc->cs = 0x08;
	proc->ds = 0x10;
	proc->kernel = true;
	proc->name = "idle";
	proc->cr3 = mm_virtual_get_physical(0xFFFFF000);
	kernel_pd = proc->cr3;

	current = thread;

	proc_create("init", false);

	return;
}

void proc_create(const char *path, bool kernel) {
	proc_t *proc = malloc(sizeof(proc_t));

	uintptr_t *pd = mm_alloc(0x1000, PAGE_PRESENT | PAGE_WRITE, true);
	uintptr_t pd_phys = mm_virtual_get_physical((uintptr_t) pd);
	uintptr_t *pd_k = (uintptr_t *) 0xFFFFF000;

	memset(pd, 0x00, 0x1000);

	for(size_t i = 0x300; i < 0x3FF; i++) {
		pd[i] = pd_k[i];
	}

	pd[0x3FF] = pd_phys | PAGE_PRESENT | PAGE_WRITE | PAGE_USER;

	mm_virtual_switch(pd_phys);

	void *heap_phys = mm_physical_alloc();
	proc->heap = (void *) 0x80000000;

	mm_virtual_map_page((uintptr_t) proc->heap, (uintptr_t) heap_phys, PAGE_PRESENT | PAGE_WRITE);

	alloc_node_t *alloc = (alloc_node_t *) proc->heap;
	memset(alloc, 0x00, sizeof(*alloc));
	alloc->next = 0;
	alloc->prev = 0;
	alloc->state = FREE;
	alloc->start = (uintptr_t) alloc + 0x1000;
	alloc->end = 0xBFFFFFFF;

	proc->thread_list = malloc(sizeof(list_t));
	thread_t *thread = thread_create(proc, (uintptr_t) proc_idle);
	list_node_t *tnode = malloc(sizeof(tnode));
	thread->esp = ((uintptr_t) (uintptr_t *) mm_alloc_proc(proc->heap, 0x1000, PAGE_PRESENT | PAGE_WRITE | PAGE_USER, true)) + 0x1000 - 4;
	thread->ebp = thread->esp;
	tnode->data = thread;
	list_append(proc->thread_list, tnode);

	uintptr_t entry = elf32_load(path);

	thread->eip = entry;
	proc->pid = pid++;
	proc->name = path;
	proc->kernel = kernel;
	proc->cs = 0x1B;
	proc->ds = 0x23;
	proc->cr3 = pd_phys;

	list_append(queue, tnode);

	mm_virtual_switch(kernel_pd);
}

noreturn uintptr_t proc_exit(syscall_args_t *data) {
	proc_t *p = current->proc;

	printf("[proc]	process %s (%u) exited with status %u\n", p->name, p->pid, data->arg1);

	current->state = THREAD_DEAD;

	asm volatile ("sti");
	asm volatile ("hlt");

	for(;;);
}

void proc_switch(frame_t *frame) {
	if(!queue->head) {
		return;
	}

	thread_t *next = queue->head->data;
	proc_t *nextp = next->proc;
	proc_t *currentp = current->proc;

	assert(next != current);
	assert(current->state == THREAD_RUNNING || current->state == THREAD_DEAD);

	/* save registers of preemted thread */
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

	/* pop the next process off the queue */
	list_node_t *node = queue->head;
	list_remove(queue, queue->head);

	if(current->state != THREAD_DEAD) {
		/* queue the preemted process */
		node->data = current;
		list_append(queue, node);

		/* mark it as suspended */
		current->state = THREAD_SUSPENDED;
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

	mm_virtual_switch(nextp->cr3);

	current = next;
}

uintptr_t sys_getpid(syscall_args_t *data) {
	(void) data;

	return current->proc->pid;
}
