#include <init/panic.h>
#include <int/handler.h>
#include <util/trace.h>

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
	trace(20);

	if(handlers[state->interrupt] == NULL) {
		if(state->interrupt == 0x0D && state->error != 0) {
			printf("offending segment selector: %#08x\n", (state->error >> 3) & 0x1FFF);
		}

		panic("unhandled interrupt %#02x (%s, error: %#08x, eip: %#08x)", state->interrupt, exceptions[state->interrupt], state->error, state->eip);
	} else {
		(*handlers[state->interrupt])(state);
	}
}

void handler_set(size_t n, isr_handler_t addr) {
	handlers[n] = addr;
}
