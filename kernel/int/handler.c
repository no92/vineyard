#include <init/panic.h>
#include <int/handler.h>
#include <int/pic.h>
#include <util/trace.h>

#include <stdio.h>

static const char *exceptions[33] = {
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
	"Unknown",
};

isr_handler_t handlers[256];

void handler(frame_t *state) {
	if(handlers[state->interrupt] == NULL) {
		size_t offset = (state->interrupt < 32) ? state->interrupt : 32;
		printf("unhandled interrupt %#02x (%s, error: %#08x, eip: %#08x)\n", state->interrupt, exceptions[offset], state->error, state->eip);

		if(state->interrupt == 0x0D && state->error != 0) {
			printf("offending segment selector: %#08x\n", (state->error >> 3) & 0x1FFF);
		}

		if(state->interrupt == 0x0E) {
			uint32_t val;
			asm volatile ("mov %%cr2, %0" : "=r" (val));
			printf("offending linear address: %#x\n", val);
		}

		trace(20);
		panic("unhandled interrupt");
	} else {
		(*handlers[state->interrupt])(state);
	}

	if(state->interrupt > 31 && state->interrupt < 48) {
		pic_eoi((state->interrupt - 32) & 0xFF);
	}
}

void handler_set(size_t n, isr_handler_t addr) {
	handlers[n] = addr;
}
