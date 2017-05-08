#include <gfx/gfx.h>
#include <stdarg.h>
#include <stdio.h>

int vprintf(const char * restrict format, va_list arg) {
	char buf[1024];
	size_t off = 0;

	int ret = vsnprintf(buf, 1024, format, arg);

	while(buf[off]) {
		off += gfx_putc(&buf[off]);
	}

	return ret;
}
