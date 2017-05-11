#ifdef __libk
	#include <gfx/gfx.h>
#endif

#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>

#define PRINTF_BUF_SIZE 1024

int printf(const char * restrict format, ...) {
	char buf[PRINTF_BUF_SIZE];
	size_t off = 0;
	va_list args;

	va_start(args, format);

	int ret = vsnprintf(buf, PRINTF_BUF_SIZE, format, args);

	va_end(args);

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
