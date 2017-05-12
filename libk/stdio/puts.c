#ifdef __libk
	#include <driver/uart.h>
	#include <gfx/gfx.h>
#endif

#include <stdio.h>

int puts(const char *str) {
	const char *s = str;

	while(*s) {
#ifdef __libk
		s += gfx_putc((char *) s);
#endif
	}

#ifdef __libk
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
