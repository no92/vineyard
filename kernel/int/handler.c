#include <init/panic.h>
#include <int/handler.h>

#include <stdio.h>

static const char *exceptions[32] = {
	"Divide Error",
	"Debug",
	"NMI",
	"Breakpoint",
	"Overflow",
	"Bound Range exceeded",
	"Invalid Opcode",
	"Device not available",
	"Double Fault",
	"Coprocessor Segment Overrun",
	"Invalid TSS",
	"Segment not Present",
	"Stack Fault",
	"General Protection",
	"Page Fault",
	"x87 Floating-Point",
	"Alignment Check",
	"Machine Check",
	"SIMD Floating-Point",
	"Virtualization",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
};

isr_handler_t handlers[32];

void handler(frame_t *state) {
	if(handlers[state->interrupt] == NULL) {
		panic("unhandled interrupt %#02x (%s)", state->interrupt, exceptions[state->interrupt]);
	} else {
		(*handlers[state->interrupt])(state);
	}
}

void handler_set(size_t n, isr_handler_t addr) {
	handlers[n] = addr;
}
