#include <init/panic.h>
#include <util/trace.h>

#include <stdio.h>

noreturn void panic(const char * restrict format, ...) {
	trace(20);

	va_list args;
	va_start(args, format);

	vprintf(format, args);
	puts("");

	va_end(args);

	asm volatile ("cli; hlt;");
	for(;;);

	__builtin_unreachable();
}
