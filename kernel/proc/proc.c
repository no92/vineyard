#include <_/vineyard.h>
#include <fs/initrd.h>
#include <mm/alloc.h>
#include <mm/physical.h>
#include <mm/virtual.h>
#include <proc/elf32.h>
#include <proc/proc.h>
#include <init/panic.h>
#include <util/list.h>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static pid_t pid = 1;
static list_t *queue;
static proc_t *current;
static uintptr_t kernel_pd;

extern uintptr_t stack_top;

static noreturn void proc_idle(void) {
	while(1) asm volatile ("hlt");
}

proc_t *proc_get(void) {
	return current;
}

void proc_init(void) {
	queue = malloc(sizeof(*queue));

	proc_t *proc = malloc(sizeof(proc_t));
	proc->pid = pid++;
	proc->esp = 0;
	proc->ebp = 0;
	proc->eip = 0;
	proc->stack = (uintptr_t) &stack_top;
	proc->eip = (uintptr_t) proc_idle;
	proc->cs = 0x08;
	proc->ds = 0x10;
	proc->kernel = true;
	proc->name = "idle";
	proc->state = PROC_RUNNING;
	proc->cr3 = mm_virtual_get_physical(0xFFFFF000);
	kernel_pd = proc->cr3;
	current = proc;

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

	proc->esp = ((uintptr_t) (uintptr_t *) mm_alloc_proc(proc->heap, 0x1000, PAGE_PRESENT | PAGE_WRITE | PAGE_USER, true)) + 0x1000 - 4;
	proc->ebp = proc->esp;
	proc->eip = header->e_entry;
	proc->pid = pid++;
	proc->name = path;
	proc->kernel = kernel;
	proc->cs = 0x1B;
	proc->ds = 0x23;
	proc->state = PROC_SUSPENDED;
	proc->cr3 = pd_phys;

	list_node_t *node = malloc(sizeof(list_node_t));
	node->data = proc;
	list_append(queue, node);

	mm_virtual_switch(kernel_pd);
}

uintptr_t proc_exit(syscall_args_t *data) {
	printf("[proc]	process %s (%u) exited with status %u\n", current->name, current->pid, data->arg1);

	current->state = PROC_DEAD;

	return 0;
}

void proc_switch(frame_t *frame) {
	asm volatile ("sti");

	if((!queue->head && current->state != PROC_DEAD) || queue->head->data == current) {
		return;
	}

	proc_t *next = queue->head->data;

	current->esp = frame->esp;
	current->ebp = frame->ebp;
	current->eip = frame->eip;
	current->cs = frame->cs;
	current->ds = frame->ds;

	if(current->state == PROC_DEAD) {
		list_remove(queue, queue->head);
		free(current);
	} else if(next->state == PROC_SUSPENDED) {
		list_node_t *node = queue->head;
		list_remove(queue, queue->head);

		current->state = PROC_SUSPENDED;
		node->data = current;
		list_append(queue, node);
	} else {
		panic("[sched]	unknown process state");
	}

	current = next;
	current->state = PROC_RUNNING;

	frame->eip = current->eip;
	frame->esp = current->esp;
	frame->ebp = current->ebp;
	frame->cs = current->cs;
	frame->ds = current->ds;
	frame->es = current->ds;
	frame->fs = current->ds;
	frame->gs = current->ds;
	frame->ss = current->ds;
	frame->eflags |= 0x200;

	mm_virtual_switch(current->cr3);
}

uintptr_t sys_getpid(syscall_args_t *data) {
	(void) data;

	return current->pid;
}
