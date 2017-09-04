#include <proc/proc.h>
#include <proc/syscall.h>
#include <int/handler.h>

#include <stdio.h>
#include <string.h>

static void syscall_handler(frame_t *frame) {
	syscall_t data;

	data.arg1 = frame->ebx;
	data.arg2 = frame->ecx;
	data.arg3 = frame->edx;
	data.arg4 = frame->esi;
	data.arg5 = frame->edi;

	if(frame->eax == 1) {
		printf("%s", (char *) frame->ebx);
	} else if(frame->eax == 2) {
		proc_exit(&data);
	}
}

void syscall_init(void) {
	handler_set(0x80, &syscall_handler);
}
