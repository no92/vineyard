#ifdef __libk
	#include <gfx/gfx.h>
#else
	#include <_/syscall.h>
	#include <stdlib.h>
#endif

#include <stdarg.h>
#include <stdio.h>

#define PRINTF_BUF_SIZE 1024

int vprintf(const char * restrict format, va_list arg) {
	char buf[PRINTF_BUF_SIZE];

	int ret = vsnprintf(buf, PRINTF_BUF_SIZE, format, arg);

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
