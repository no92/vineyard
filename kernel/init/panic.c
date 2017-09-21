#include <init/panic.h>
#include <util/trace.h>

#include <stdio.h>

noreturn void panic(const char * restrict format, ...) {
	asm volatile ("cli");

	trace(20);

	va_list args;
	va_start(args, format);

	vprintf(format, args);
	puts("");

	va_end(args);

	asm volatile ("hlt;");
	for(;;);

	__builtin_unreachable();
}
