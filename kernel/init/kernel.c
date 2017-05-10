#include <driver/uart.h>
#include <gfx/gfx.h>
#include <mm/gdt.h>
#include <assert.h>
#include <stdio.h>

void kernel(uint32_t magic, multiboot2_t *multiboot);

void kernel(uint32_t magic, multiboot2_t *multiboot) {
	gfx_init(multiboot);
	uart_init();

	gdt_init();

	printf("vineyard\n");

	assert(magic == MB2_MAGIC);
}
