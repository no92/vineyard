#include <driver/uart.h>
#include <gfx/gfx.h>

#include <_/vineyard.h>
#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

static uint32_t gfx_x = 0;
static uint32_t gfx_y = 0;
static uint32_t gfx_char_width = 8;
static uint32_t gfx_char_height = 16;
static uint32_t gfx_rows = 0;
static uint32_t gfx_lines = 0;

static void gfx_refresh(void) {
	if(gfx_rows == 0 && gfx_lines == 0) {
		gfx_rows = gfx_width / gfx_char_width;
		gfx_lines = gfx_height / gfx_char_height;
	}

	uint32_t bytes_per_line = gfx_pitch * gfx_char_height;

	if(gfx_x >= gfx_width) {
		gfx_x = 0;
		gfx_y += gfx_char_height;
	}

	/* check whether we have to scroll */
	if(gfx_y >= gfx_height - gfx_char_height) {
		uint8_t *start = (uint8_t *) gfx_framebuffer;

		for(size_t line = 0; line < gfx_lines; line++) {
			memcpy(start, start + bytes_per_line, bytes_per_line);

			start += bytes_per_line;
		}

		memset(start - bytes_per_line, 0, bytes_per_line);

		gfx_y -= gfx_char_height;
	}
}

A("goto statement")
size_t gfx_putc(const char *c) {
	if(c[0] == '\n') {
		gfx_x = 0;
		gfx_y += gfx_char_height;

		goto exit;
	} else if(c[0] == '\t') {
		/* move gfx_x to the next-highest multiple of 4 */
		gfx_x = (gfx_x + (gfx_char_width << 2)) & ~((gfx_char_width << 2) - 1);

		goto exit;
	} else if(c[0] == '\b') {
		/* move the x coordinate 1 character to the left, but not off the screen */
		gfx_x = (gfx_x <= gfx_char_width) ? 0 : gfx_x - gfx_char_width;
		/* clear the character we deleted */
		gfx_draw_char(" ", gfx_x, gfx_y);

		/* for some reason, my GNOME terminal needs two backspaces to move back one char */
		uart_putc(0x08);
		uart_putc(0x08);
		/* we want a destructive backspace, so set it up ... */
		uart_putc(0x20);

		/* ... and complete it, including cleanup */
		goto exit;
	}

	gfx_refresh();
	size_t ret = gfx_draw_char(c, gfx_x, gfx_y);
	gfx_x += gfx_char_width;

	return ret;
exit:
	gfx_refresh();
	uart_putc((uint8_t) c[0]);
	return 1;
}
