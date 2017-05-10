#include <int/idt.h>
#include <init/panic.h>
#include <stdio.h>
#include <string.h>

#define IDT_ENTRIES		256

#define IDT_PRESENT		0x80
#define IDT_INTERRUPT	0x0E

idt_entry_t idt[IDT_ENTRIES];
idtr_t idtr;

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

static void idt_set(size_t interrupt, void (*function)(void), uint16_t selector, uint8_t flags) {
	idt[interrupt].offset_low = ((uintptr_t) function) & 0xFFFF;
	idt[interrupt].offset_high = (uint16_t) ((uintptr_t) function >> 16) & 0xFFFF;

	idt[interrupt].selector = selector;
	idt[interrupt].flags = flags;
}

void handler(frame_t *state) {
	panic("unhandled interrupt %#02x (%s)", state->interrupt, exceptions[state->interrupt]);
}

void idt_init(void) {
	memset(idt, 0, sizeof(idt));

	idt_set(0, &exception0, 0x08, IDT_PRESENT | IDT_INTERRUPT);
	idt_set(1, &exception1, 0x08, IDT_PRESENT | IDT_INTERRUPT);
	idt_set(2, &exception2, 0x08, IDT_PRESENT | IDT_INTERRUPT);
	idt_set(3, &exception3, 0x08, IDT_PRESENT | IDT_INTERRUPT);
	idt_set(4, &exception4, 0x08, IDT_PRESENT | IDT_INTERRUPT);
	idt_set(5, &exception5, 0x08, IDT_PRESENT | IDT_INTERRUPT);
	idt_set(6, &exception6, 0x08, IDT_PRESENT | IDT_INTERRUPT);
	idt_set(7, &exception7, 0x08, IDT_PRESENT | IDT_INTERRUPT);
	idt_set(8, &exception8, 0x08, IDT_PRESENT | IDT_INTERRUPT);
	idt_set(9, &exception9, 0x08, IDT_PRESENT | IDT_INTERRUPT);
	idt_set(10, &exception10, 0x08, IDT_PRESENT | IDT_INTERRUPT);
	idt_set(11, &exception11, 0x08, IDT_PRESENT | IDT_INTERRUPT);
	idt_set(12, &exception12, 0x08, IDT_PRESENT | IDT_INTERRUPT);
	idt_set(13, &exception13, 0x08, IDT_PRESENT | IDT_INTERRUPT);
	idt_set(14, &exception14, 0x08, IDT_PRESENT | IDT_INTERRUPT);
	idt_set(15, &exception15, 0x08, IDT_PRESENT | IDT_INTERRUPT);
	idt_set(16, &exception16, 0x08, IDT_PRESENT | IDT_INTERRUPT);
	idt_set(17, &exception17, 0x08, IDT_PRESENT | IDT_INTERRUPT);
	idt_set(18, &exception18, 0x08, IDT_PRESENT | IDT_INTERRUPT);
	idt_set(19, &exception19, 0x08, IDT_PRESENT | IDT_INTERRUPT);
	idt_set(20, &exception20, 0x08, IDT_PRESENT | IDT_INTERRUPT);
	idt_set(21, &exception21, 0x08, IDT_PRESENT | IDT_INTERRUPT);
	idt_set(22, &exception22, 0x08, IDT_PRESENT | IDT_INTERRUPT);
	idt_set(23, &exception23, 0x08, IDT_PRESENT | IDT_INTERRUPT);
	idt_set(24, &exception24, 0x08, IDT_PRESENT | IDT_INTERRUPT);
	idt_set(25, &exception25, 0x08, IDT_PRESENT | IDT_INTERRUPT);
	idt_set(26, &exception26, 0x08, IDT_PRESENT | IDT_INTERRUPT);
	idt_set(27, &exception27, 0x08, IDT_PRESENT | IDT_INTERRUPT);
	idt_set(28, &exception28, 0x08, IDT_PRESENT | IDT_INTERRUPT);
	idt_set(29, &exception29, 0x08, IDT_PRESENT | IDT_INTERRUPT);
	idt_set(30, &exception30, 0x08, IDT_PRESENT | IDT_INTERRUPT);
	idt_set(31, &exception31, 0x08, IDT_PRESENT | IDT_INTERRUPT);

	idtr.limit = sizeof(idt) - 1;
	idtr.addr = (uintptr_t) &idt;

	asm volatile ("lidt %0" : : "m" (idtr));
	asm volatile ("sti");
}
