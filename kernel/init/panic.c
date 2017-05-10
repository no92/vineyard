#include <init/panic.h>
#include <stdarg.h>
#include <stdio.h>

noreturn void panic(const char * restrict format, ...) {
	va_list args;
	va_start(args, format);

	vprintf(format, args);

	va_end(args);

	asm volatile ("cli; hlt;");
	for(;;);

	__builtin_unreachable();
}
