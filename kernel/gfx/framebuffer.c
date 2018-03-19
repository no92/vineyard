#include <_/vineyard.h>
#include <driver/uart.h>
#include <gfx/gfx.h>

static size_t offset_1;
static size_t offset_2;
static size_t offset_3;
static size_t offset_4;
static size_t offset_5;
static size_t offset_6;
static size_t offset_7;

void gfx_init(multiboot2_t *multiboot) {
	multiboot2_tag_t *video = multiboot2_get_tag(multiboot, MB2_TYPE_FRAMEBUFFER);

	gfx_framebuffer = (uint32_t *) video->video.framebuffer_addr;
	gfx_width = video->video.framebuffer_width;
	gfx_height = video->video.framebuffer_height;
	gfx_pitch = video->video.framebuffer_pitch;
	gfx_bpp = video->video.framebuffer_bpp >> 3;
	gfx_framebuffer_size = gfx_pitch * gfx_height;

	offset_1 = gfx_bpp;
	offset_2 = gfx_bpp << 1;
	offset_3 = offset_1 + offset_2;
	offset_4 = gfx_bpp << 2;
	offset_5 = offset_4 + offset_1;
	offset_6 = offset_4 + offset_2;
	offset_7 = offset_4 + offset_3;
}

void gfx_set(size_t h, size_t v, uint32_t color) {
	uint32_t *addr = gfx_framebuffer + LFB(h, v);
	*addr = color;
}

A("long method")
size_t gfx_draw_char(const char *c, size_t x, size_t y) {
	char *addr = (char *) gfx_framebuffer + LFB(x, y);
	uint32_t index = 0;
	size_t ret = 0;

	char c0 = (char) (c[0] & 0xFF);

	/* handle UTF-8 multi-byte characters correctly */
	if((c0 & 0x80) == 0) {
		/* single-byte */
		ret = 1;
		index = (uint32_t) c0;

		uart_putc((uint8_t) c[0]);
	} else if((c0 & 0xE0) == 0xC0) {
		/* 2 bytes */
		char c1 = c[1] & 0x3F;
		ret = 2;
		index = (uint32_t) (((c0 & 0x1F) << 6) | c1);

		uart_putc((uint8_t) c[0]);
		uart_putc((uint8_t) c[1]);
	} else if((c0 & 0xF0) == 0xE0) {
		/* 3 bytes */
		char c1 = (c[1] & 0xFF) & 0x3F;
		char c2 = (c[2] & 0xFF) & 0x3F;
		ret = 3;
		index = (uint32_t) (((c0 & 0x0F) << 12) | (c1 << 6) | c2);

		uart_putc((uint8_t) c[0]);
		uart_putc((uint8_t) c[1]);
		uart_putc((uint8_t) c[2]);
	} else if((c0 & 0xF8) == 0xF0) {
		/* 4 bytes */
		char c1 = (c[1] & 0xFF) & 0x3F;
		char c2 = (c[2] & 0xFF) & 0x3F;
		char c3 = (c[3] & 0xFF) & 0x3F;
		ret = 4;
		index = (uint32_t) (((c0 & 0x07) << 18) | (c1 << 12) | (c2 << 6) | c3);

		uart_putc((uint8_t) c[0]);
		uart_putc((uint8_t) c[1]);
		uart_putc((uint8_t) c[2]);
		uart_putc((uint8_t) c[3]);
	}

	if(index >= 64256) {
		index = (uint32_t) (gfx_font_index[index-54416] << 4);
	} else {
		index = (uint32_t) (gfx_font_index[index] << 4);
	}

	uint8_t *char_data = (uint8_t *) &gfx_font[index];
	uint8_t row_data;

	uint32_t mask_0;
	uint32_t mask_1;
	uint32_t mask_2;
	uint32_t mask_3;
	uint32_t mask_4;
	uint32_t mask_5;
	uint32_t mask_6;
	uint32_t mask_7;

	uint32_t fg = 0xFFFFFFFF;
	uint32_t bg = 0x00000000;

	for(int row = 0; row < 16; row++) {
		row_data = char_data[row];

		/* the leftmost pixel is represented by the MSB */
		mask_0 = gfx_font_lookup[(row_data >> 7) & 1];
		mask_1 = gfx_font_lookup[(row_data >> 6) & 1];
		mask_2 = gfx_font_lookup[(row_data >> 5) & 1];
		mask_3 = gfx_font_lookup[(row_data >> 4) & 1];
		mask_4 = gfx_font_lookup[(row_data >> 3) & 1];
		mask_5 = gfx_font_lookup[(row_data >> 2) & 1];
		mask_6 = gfx_font_lookup[(row_data >> 1) & 1];
		mask_7 = gfx_font_lookup[(row_data >> 0) & 1];

		*(uint32_t *) addr = (fg & mask_0) | (bg & ~mask_0);
		*(uint32_t *) (&addr[offset_1]) = (fg & mask_1) | (bg & ~mask_1);
		*(uint32_t *) (&addr[offset_2]) = (fg & mask_2) | (bg & ~mask_2);
		*(uint32_t *) (&addr[offset_3]) = (fg & mask_3) | (bg & ~mask_3);
		*(uint32_t *) (&addr[offset_4]) = (fg & mask_4) | (bg & ~mask_4);
		*(uint32_t *) (&addr[offset_5]) = (fg & mask_5) | (bg & ~mask_5);
		*(uint32_t *) (&addr[offset_6]) = (fg & mask_6) | (bg & ~mask_6);
		*(uint32_t *) (&addr[offset_7]) = (fg & mask_7) | (bg & ~mask_7);

		addr += gfx_pitch;
	}

	return ret;
}

uint32_t gfx_font_lookup[2] = {
    0x00000000,
    0xFFFFFFFF,
};
