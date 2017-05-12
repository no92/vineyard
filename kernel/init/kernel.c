#include <driver/uart.h>
#include <gfx/gfx.h>
#include <int/idt.h>
#include <int/pic.h>
#include <mm/gdt.h>
#include <assert.h>
#include <stdio.h>

void kernel(uint32_t magic, multiboot2_t *multiboot);

void kernel(uint32_t magic, multiboot2_t *multiboot) {
	gfx_init(multiboot);
	uart_init();

	assert(magic == MB2_MAGIC);

	gdt_init();
	idt_init();
	pic_init();

	asm volatile ("sti");

	printf("vineyard\n");
}
