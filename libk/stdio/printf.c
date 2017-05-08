#include <gfx/gfx.h>
#include <stdarg.h>
#include <stdio.h>

int printf(const char * restrict format, ...) {
	char buf[1024];
	size_t off = 0;
	va_list args;

	va_start(args, format);

	int ret = vsnprintf(buf, 1024, format, args);

	va_end(args);

	while(buf[off]) {
		off += gfx_putc(&buf[off]);
	}

	return ret;
}
