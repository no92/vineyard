#include <int/idt.h>
#include <int/handler.h>
#include <init/panic.h>
#include <util/trace.h>

#include <_/vineyard.h>
#include <stdio.h>
#include <stdnoreturn.h>
#include <string.h>

#define IDT_PRESENT		0x80
#define IDT_INTERRUPT	0x0E
#define IDT_USER		0x60

idt_entry_t idt[IDT_ENTRIES];
idtr_t idtr;

static void idt_set(size_t interrupt, void (*function)(void), uint16_t selector, uint8_t flags) {
	idt[interrupt].offset_low = ((uintptr_t) function) & 0xFFFF;
	idt[interrupt].offset_high = (uint16_t) ((uintptr_t) function >> 16) & 0xFFFF;

	idt[interrupt].selector = selector;
	idt[interrupt].flags = flags;
}

static noreturn void idt_handler_gpf(frame_t *frame) {
	char buf[0x20];

	if(frame->error != 0) {
		const char *table = (frame->error & 0x01) ? "IDT entry" : "GDT entry";

		snprintf(buf, 0x20, " with offending %s %#02x", table, (frame->error >> 3));
	}

	panic("General Protection Fault at address %#08x <%s>%s", frame->eip, trace_lookup_addr(frame->eip), buf);
}

void idt_init(void) {
	memset(idt, 0, sizeof(idt));

	/* set up exception handlers */
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

	/* set up IRQ handlers */
	idt_set(32, &irq0, 0x08, IDT_PRESENT | IDT_INTERRUPT);
	idt_set(33, &irq1, 0x08, IDT_PRESENT | IDT_INTERRUPT);
	idt_set(34, &irq2, 0x08, IDT_PRESENT | IDT_INTERRUPT);
	idt_set(35, &irq3, 0x08, IDT_PRESENT | IDT_INTERRUPT);
	idt_set(36, &irq4, 0x08, IDT_PRESENT | IDT_INTERRUPT);
	idt_set(37, &irq5, 0x08, IDT_PRESENT | IDT_INTERRUPT);
	idt_set(38, &irq6, 0x08, IDT_PRESENT | IDT_INTERRUPT);
	idt_set(39, &irq7, 0x08, IDT_PRESENT | IDT_INTERRUPT);
	idt_set(40, &irq8, 0x08, IDT_PRESENT | IDT_INTERRUPT);
	idt_set(41, &irq9, 0x08, IDT_PRESENT | IDT_INTERRUPT);
	idt_set(42, &irq10, 0x08, IDT_PRESENT | IDT_INTERRUPT);
	idt_set(43, &irq11, 0x08, IDT_PRESENT | IDT_INTERRUPT);
	idt_set(44, &irq12, 0x08, IDT_PRESENT | IDT_INTERRUPT);
	idt_set(45, &irq13, 0x08, IDT_PRESENT | IDT_INTERRUPT);
	idt_set(46, &irq14, 0x08, IDT_PRESENT | IDT_INTERRUPT);
	idt_set(47, &irq15, 0x08, IDT_PRESENT | IDT_INTERRUPT);

	idt_set(0x80, &handle_syscall, 0x08, IDT_PRESENT | IDT_INTERRUPT | IDT_USER);

	/* set up the IDT pointer */
	idtr.limit = sizeof(idt) - 1;
	idtr.addr = (uintptr_t) &idt;

	/* load the IDT */
	asm volatile ("lidt %0" : : "m" (idtr));

	handler_set(0x0D, idt_handler_gpf);
}
