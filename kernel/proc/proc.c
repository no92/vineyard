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

static pid_t pid = 1;
static list_t *queue;
static thread_t *current;
static uintptr_t kernel_pd;

extern uintptr_t stack_top;

static noreturn void proc_idle(void) {
	while(1) asm volatile ("hlt");
}

proc_t *proc_get(void) {
	return (proc_t *) current->proc;
}

void proc_init(void) {
	queue = malloc(sizeof(*queue));

	proc_t *proc = malloc(sizeof(proc_t));
	proc->pid = pid++;
	proc->heap = mm_alloc_areas;
	proc->thread_list = malloc(sizeof(list_t));

	thread_t *thread = thread_create(proc, (uintptr_t) proc_idle);
	list_node_t *tnode = malloc(sizeof(list_node_t));
	thread->esp = 0;
	thread->ebp = 0;
	thread->eip = (uintptr_t) proc_idle;
	tnode->data = thread;
	list_append(proc->thread_list, tnode);

	proc->cs = 0x08;
	proc->ds = 0x10;
	proc->kernel = true;
	proc->name = "idle";
	proc->state = PROC_RUNNING;
	proc->cr3 = mm_virtual_get_physical(0xFFFFF000);
	kernel_pd = proc->cr3;

	current = thread;

	proc_create("init", false);

	return;
}

void proc_create(const char *path, bool kernel) {
	initrd_file_t *file = initrd_open(path);
	proc_t *proc = malloc(sizeof(proc_t));

	uintptr_t *pd = mm_alloc(0x1000, PAGE_PRESENT | PAGE_WRITE | PAGE_USER, true);
	uintptr_t pd_phys = mm_virtual_get_physical((uintptr_t) pd);
	uintptr_t *pd_k = (uintptr_t *) 0xFFFFF000;

	memset(pd, 0x00, 0x1000);

	for(size_t i = 0x00; i < 0x400; i++) {
		pd[i] = pd_k[i];
	}

	pd[0x3FF] = mm_virtual_get_physical((uintptr_t) pd) | PAGE_PRESENT | PAGE_WRITE | PAGE_USER;

	if(!file) {
		panic("process \"%s\" not found!", path);
	}

	mm_virtual_switch(pd_phys);

	elf32_header_t *header = (elf32_header_t *) malloc(initrd_file_size(file->size));
	uintptr_t file_start = ALIGN_UP((uintptr_t) file + sizeof(*file), 0x200);
	memcpy(header, (void *) file_start, initrd_file_size(file->size));

	assert(header->e_ident[0] == 0x7F && header->e_ident[1] == 'E' && header->e_ident[2] == 'L' && header->e_ident[3] == 'F');

	for(uintptr_t x = 0; x < (uint32_t) header->e_shentsize * header->e_shnum; x += header->e_shentsize) {
		elf32_shdr_t *shdr = (elf32_shdr_t *) ((uintptr_t) header + header->e_shoff + x);

		if(!shdr->sh_addr) {
			continue;
		}

		if(!(mm_virtual_get_physical(shdr->sh_addr) & 0xFFFFF000)) {
			uintptr_t phys = (uintptr_t) (uintptr_t *) mm_physical_alloc();
			mm_virtual_map_page(shdr->sh_addr & 0xFFFFF000, phys, PAGE_PRESENT | PAGE_WRITE | PAGE_USER);
		}

		memcpy((void *) (shdr->sh_addr), (void *) ((uintptr_t) header + shdr->sh_offset), shdr->sh_size);
	}

	mm_virtual_map_page(0x80000000, (uintptr_t) (uintptr_t *) mm_physical_alloc(), PAGE_PRESENT | PAGE_WRITE);
	proc->heap = (void *) 0x80000000;
	alloc_node_t *alloc = (alloc_node_t *) 0x80000000;
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
	thread->eip = header->e_entry;
	tnode->data = thread;
	list_append(proc->thread_list, tnode);

	proc->pid = pid++;
	proc->name = path;
	proc->kernel = kernel;
	proc->cs = 0x1B;
	proc->ds = 0x23;
	proc->state = PROC_SUSPENDED;
	proc->cr3 = pd_phys;

	list_append(queue, tnode);

	mm_virtual_switch(kernel_pd);
}

uintptr_t proc_exit(syscall_args_t *data) {
	proc_t *p = current->proc;

	printf("[proc]	process %s (%u) exited with status %u\n", p->name, p->pid, data->arg1);

	p->state = PROC_DEAD;

	return 0;
}

void proc_switch(frame_t *frame) {
	asm volatile ("sti");

	thread_t *next = queue->head->data;
	proc_t *nextp = next->proc;
	proc_t *currentp = current->proc;

	if((!queue->head && currentp->state != PROC_DEAD) || queue->head->data == current) {
		return;
	}

	current->esp = frame->esp;
	current->ebp = frame->ebp;
	current->eip = frame->eip;
	currentp->cs = frame->cs;
	currentp->ds = frame->ds;

	if(currentp->state == PROC_DEAD) {
		list_remove(queue, queue->head);
		free(current);
	} else if(nextp->state == PROC_SUSPENDED) {
		list_node_t *node = queue->head;
		list_remove(queue, queue->head);

		currentp->state = PROC_SUSPENDED;
		node->data = current;
		list_append(queue, node);
	} else {
		panic("[sched]	unknown process state");
	}

	nextp->state = PROC_RUNNING;

	frame->eip = next->eip;
	frame->esp = next->esp;
	frame->ebp = next->ebp;
	frame->cs = nextp->cs;
	frame->ds = nextp->ds;
	frame->es = nextp->ds;
	frame->fs = nextp->ds;
	frame->gs = nextp->ds;
	frame->ss = nextp->ds;
	frame->eflags |= 0x200;

	mm_virtual_switch(nextp->cr3);
}

uintptr_t sys_getpid(syscall_args_t *data) {
	(void) data;

	return current->proc->pid;
}
