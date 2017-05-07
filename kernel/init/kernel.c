#include <driver/uart.h>
#include <gfx/gfx.h>
#include <stdio.h>

void kernel(uint32_t magic, multiboot2_t *multiboot);

void kernel(uint32_t magic, multiboot2_t *multiboot) {
	gfx_init(multiboot);
	uart_init();

	puts("vineyard");

	if(magic != MULTIBOOT2_MAGIC) {
		puts("[panic]	invalid multiboot2 magic");
		return;
	}
}
