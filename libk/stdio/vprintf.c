#ifdef __libk
	#include <gfx/gfx.h>
#endif

#include <stdarg.h>
#include <stdio.h>

int vprintf(const char * restrict format, va_list arg) {
	char buf[1024];
	size_t off = 0;

	int ret = vsnprintf(buf, 1024, format, arg);

	while(buf[off]) {
#ifdef __libk
		off += gfx_putc(&buf[off]);
#endif
	}

	return ret;
}

#ifdef UNIT
#include <_libk_test.h>

int main(void) {
	return EXIT_SUCCESS;
}

#endif
