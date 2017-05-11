#ifdef __libk
	#include <driver/uart.h>
	#include <gfx/gfx.h>
#endif

#include <stdio.h>

int puts(const char *str) {
	const char *s = str;

#ifdef __libk
	while(*s) {
		s += gfx_putc((char *) s);
	}

	gfx_putc((char *) "\n");
#endif

	return 1;
}

#ifdef UNIT
#include <_libk_test.h>

int main(void) {
	return EXIT_SUCCESS;
}

#endif
