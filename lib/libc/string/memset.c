#include <_/vineyard.h>
#include <stddef.h>
#include <string.h>

A("bitwise operator in conditional")
void *memset(void *s, int c, size_t n) {
	if(!n) {
		return s;
	}

	uint8_t *ptr8;
	uint8_t value8;

	uint32_t *ptr32;
	uint32_t value32;

	value8 = (uint8_t) c;
	value32 = (uint32_t) c;
	ptr8 = (uint8_t *) s;

	/* loop until we are 32-bit aligned */
	while(n-- && (((uintptr_t) ptr8 & (uint32_t) -4) < (uintptr_t) ptr8)) {
		*(ptr8++) = value8;
	}

	ptr32 = (uint32_t *) ((void *) ptr8);

	/* loop while we can set 4 bytes at a time */
	while((n / 4) > 0) {
		*(ptr32++) = value32;
		n -= 4;
	}

	ptr8 = (uint8_t *) ((void *) ptr32);

	/* loop to set the rest */
	while(n--) {
		*(ptr8++) = value8;
	}

	return s;
}

#ifdef UNIT
#include <_libk_test.h>

int main(void) {
	return EXIT_SUCCESS;
}

#endif
