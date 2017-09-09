#ifdef __libk
	#include <driver/uart.h>
	#include <gfx/gfx.h>
#else
	#include <_/syscall.h>
#endif

#include <stdio.h>
#include <string.h>

int puts(const char *str) {
#ifdef __libk
	while(*str) {
		str += gfx_putc((char *) str);
	}

	gfx_putc((char *) "\n");
#else
	size_t len = strlen(str);
	SYSCALL3(0x01, 1, str, len);
#endif

	return 1;
}

#ifdef UNIT
#include <_libk_test.h>

int main(void) {
	return EXIT_SUCCESS;
}

#endif
