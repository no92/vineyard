#include <die.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>

noreturn void die(const char *format, ...) {
	va_list args;
	va_start(args, format);

	vprintf(format, args);

	va_end(args);

	exit(1);

	__builtin_unreachable();
}
