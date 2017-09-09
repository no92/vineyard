#include <proc/proc.h>
#include <proc/syscall.h>
#include <int/handler.h>
#include <fs/vfs.h>

#include <stdio.h>
#include <string.h>

#define SYSCALLS 3

static syscall_t syscall_list[SYSCALLS] = {
	[SYSCALL_EXIT] proc_exit,
	[SYSCALL_WRITE] sys_write,
	[SYSCALL_GETPID] sys_getpid,
};

static void syscall_handler(frame_t *frame) {
	syscall_args_t data;

	data.arg1 = frame->ebx;
	data.arg2 = frame->ecx;
	data.arg3 = frame->edx;
	data.arg4 = frame->esi;
	data.arg5 = frame->edi;

	if(frame->eax >= SYSCALLS) {
		return;
	}

	syscall_t func = syscall_list[frame->eax];

	func(&data);
}

void syscall_init(void) {
	handler_set(0x80, &syscall_handler);
}
