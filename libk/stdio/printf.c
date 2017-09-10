#ifdef __libk
	#include <gfx/gfx.h>
#else
	#include <_/syscall.h>
#endif

#include <stdarg.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>

#define PRINTF_BUF_SIZE 1024

int printf(const char * restrict format, ...) {
	char buf[PRINTF_BUF_SIZE];
	va_list args;

	va_start(args, format);

	int ret = vsnprintf(buf, PRINTF_BUF_SIZE, format, args);

	va_end(args);

#ifdef __libk
	size_t off = 0;

	while(buf[off]) {
		off += gfx_putc(&buf[off]);
	}
#else
	SYSCALL3(0x01, 1, buf, min(ret, PRINTF_BUF_SIZE));
#endif

	return ret;
}


#ifdef UNIT
#include <_libk_test.h>

int main(void) {
	return EXIT_SUCCESS;
}

#endif
