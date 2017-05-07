#include <gfx/gfx.h>
#include <stdio.h>

int puts(const char *str) {
	const char *s = str;

	while(*s) {
		s += gfx_putc((char *) s);
	}

	gfx_putc((char *) "\n");

	return 1;
}
