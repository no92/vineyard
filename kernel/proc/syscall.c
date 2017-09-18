#include <fs/vfs.h>
#include <int/handler.h>
#include <mm/alloc.h>
#include <proc/proc.h>
#include <proc/syscall.h>
#include <init/panic.h>

#include <string.h>

#define SYSCALLS 5

static syscall_t syscall_list[SYSCALLS] = {
	[SYSCALL_EXIT] = proc_exit,
	[SYSCALL_WRITE] = sys_write,
	[SYSCALL_GETPID] = sys_getpid,
	[SYSCALL_MMAP] = sys_mmap,
	[SYSCALL_MUNMAP] = sys_munmap,
};

static void syscall_handler(frame_t *frame) {
	syscall_args_t data;

	data.arg1 = frame->ebx;
	data.arg2 = frame->ecx;
	data.arg3 = frame->edx;
	data.arg4 = frame->esi;
	data.arg5 = frame->edi;

	if(frame->eax >= SYSCALLS) {
		panic("invalid syscall %u", frame->eax);
		return;
	}

	syscall_t func = syscall_list[frame->eax];

	frame->eax = func(&data);
}

void syscall_init(void) {
	handler_set(0x80, &syscall_handler);
}
