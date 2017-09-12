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
	va_list args;
	va_start(args, format);

	int ret = vprintf(format, args);

	va_end(args);

	return ret;
}


#ifdef UNIT
#include <_libk_test.h>

int main(void) {
	return EXIT_SUCCESS;
}

#endif
