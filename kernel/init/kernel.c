#include <driver/keyboard.h>
#include <driver/uart.h>
#include <gfx/gfx.h>
#include <int/idt.h>
#include <int/pic.h>
#include <mm/alloc.h>
#include <mm/gdt.h>
#include <mm/map.h>
#include <mm/physical.h>
#include <mm/virtual.h>
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
	mm_physical_init(multiboot);
	mm_virtual_init();
	mm_alloc_init();

	printf("vineyard\n");

	pit_init();
	keyboard_init();

	asm volatile ("sti");

	for(;;) {
		asm volatile ("hlt");
	}
}
