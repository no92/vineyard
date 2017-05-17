#include <driver/uart.h>
#include <gfx/gfx.h>
#include <int/idt.h>
#include <int/pic.h>
#include <mm/gdt.h>
#include <mm/map.h>
#include <time/pit.h>
#include <util/trace.h>

#include <assert.h>
#include <stdio.h>
#include <stdnoreturn.h>

noreturn void kernel(uint32_t magic, multiboot2_t *multiboot);

void kernel(uint32_t magic, multiboot2_t *multiboot) {
	gfx_init(multiboot);
	uart_init();

	assert(magic == MB2_MAGIC);

	gdt_init();
	idt_init();
	pic_init();
	trace_init(multiboot);

	mm_map_init(multiboot);

	printf("vineyard\n");

	pit_init();

	asm volatile ("sti");

	for(;;) {
		asm volatile ("hlt");
	}
}
