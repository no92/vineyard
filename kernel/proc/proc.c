#include <_/vineyard.h>
#include <fs/initrd.h>
#include <int/handler.h>
#include <mm/alloc.h>
#include <mm/physical.h>
#include <mm/virtual.h>
#include <proc/elf32.h>
#include <proc/tss.h>
#include <proc/proc.h>
#include <proc/thread.h>
#include <proc/scheduler.h>
#include <init/panic.h>
#include <util/list.h>

#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* pid counter */
static pid_t pid = 1;

/* kernel proc */
static proc_t *kernel_proc;

/* holds the physical address of the kernel PD */
static uintptr_t kernel_pd;

static void proc_idle(void *data __unused) {
	while(1) asm volatile ("hlt");
}

noreturn uintptr_t proc_exit(syscall_args_t *data) {
	proc_t *p = proc_get();

	printf("[proc]	process %s (%u) exited with status %u\n", p->name, p->pid, data->arg1);

	thread_get()->state = THREAD_DEAD;

	asm volatile ("sti");
	asm volatile ("hlt");

	for(;;);
}

static void proc_worker_exit(void) {
	thread_get()->state = THREAD_DEAD;

	asm volatile ("sti");
	asm volatile ("hlt");

	for(;;);
}

static void proc_yield(frame_t *frame) {
	scheduler_tick(frame);
}

list_node_t *proc_create_worker(proc_worker_t worker, void *arg) {
	thread_t *thread = thread_create(kernel_proc, (uintptr_t) worker);
	list_node_t *tnode = malloc(sizeof(list_node_t));
	void *stack = mm_alloc_proc(mm_alloc_areas, 0x4000, PAGE_PRESENT | PAGE_WRITE, true);
	uintptr_t *stack_data = (uintptr_t *) ((uintptr_t) stack + 0x4000 - 8);

	stack_data[0] = (uintptr_t) proc_worker_exit;
	stack_data[1] = (uintptr_t) arg;

	thread->esp = (uintptr_t) stack_data;
	thread->ebp = thread->esp;
	thread->eip = (uintptr_t) worker;
	thread->state = THREAD_SUSPENDED;
	tnode->data = thread;

	list_append(kernel_proc->thread_list, tnode);
	scheduler_queue_add(tnode);

	return tnode;
}

void proc_create(const char *path, bool kernel, int argc, char *argv[], int envc, char *envp[]) {
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
	thread_t *thread = thread_create(proc, 0);
	list_node_t *tnode = malloc(sizeof(tnode));
	thread->esp = ((uintptr_t) (uintptr_t *) mm_alloc_proc(proc->heap, 0x1000, PAGE_PRESENT | PAGE_WRITE | PAGE_USER, true)) + 0x1000 - 24;
	thread->ebp = thread->esp;
	tnode->data = thread;
	list_append(proc->thread_list, tnode);

	/* copy argv and envp */
	assert(argc <= ARG_MAX);
	size_t memory = 0;

	for(int i = 0; i < argc; i++) {
		memory += strlen(argv[i]) + 1;
	}

	memory += 4;

	void *area = mm_alloc_proc(proc->heap, ALIGN_UP(memory, 0x1000), PAGE_PRESENT | PAGE_WRITE | PAGE_USER, true);
	char **u_argv = (char **) area;
	area = (void *) ((uintptr_t) area + (uintptr_t) (argc + 1) * sizeof(void *));

	for(int i = 0; i < argc; i++) {
		size_t len = strlen(argv[i]) + 1;
		u_argv[i] = area;
		strncpy(area, argv[i], len);
		area = (void *) ((uintptr_t) area + len);
	}

	uintptr_t *data = (uintptr_t *) thread->esp;
	data[0] = (uintptr_t) envc;
	data[1] = (uintptr_t) envp;
	data[2] = (uintptr_t) argc;
	data[3] = (uintptr_t) u_argv;
	data[4] = 0;
	data[5] = 0;

	uintptr_t entry = elf32_load(path);

	thread->eip = entry;
	proc->pid = pid++;
	proc->name = path;
	proc->kernel = kernel;
	proc->cs = 0x1B;
	proc->ds = 0x23;
	proc->cr3 = pd_phys;

	scheduler_queue_add(tnode);

	mm_virtual_switch(kernel_pd);
}

uintptr_t sys_getpid(syscall_args_t *data) {
	(void) data;

	return proc_get()->pid;
}

void proc_init(void) {
	/* set up the kernel process */
	kernel_proc = malloc(sizeof(proc_t));
	kernel_proc->pid = 0;
	kernel_proc->heap = mm_alloc_areas;
	kernel_proc->thread_list = malloc(sizeof(list_t));
	kernel_proc->cs = 0x08;
	kernel_proc->ds = 0x10;
	kernel_proc->kernel = true;
	kernel_proc->name = "kernel";
	kernel_proc->cr3 = mm_virtual_get_physical(0xFFFFF000);
	kernel_pd = kernel_proc->cr3;

	scheduler_init();

	list_node_t *tnode = proc_create_worker(proc_idle, NULL);
	thread_t *idle = tnode->data;
	idle->state = THREAD_RUNNING;

	scheduler_queue_remove(tnode);
	scheduler_set_idle(idle);

	handler_set(0x81, proc_yield);

	char *argv[] = {(char *) "init", 0};
	char *envp[] = {0};
	proc_create("init", false, 1, argv, 0, envp);

	return;
}
