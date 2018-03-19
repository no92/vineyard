#include <stddef.h>
#include <string.h>

void *memccpy(void * restrict dest, const void * restrict src, int c, size_t n) {
	uint8_t *d = (uint8_t *) dest;
	const uint8_t *s = (uint8_t *) src;

	for(size_t i = 0; i < n; i++) {
		/* copy character by character, checking for c to occur */
		if((d[i] = s[i]) == (uint8_t) c) {
			return d + i + 1;
		}
	}

	/* if we didn't find c in the buffer, then return NULL */
	return NULL;
}

#ifdef UNIT
#include <_libk_test.h>

int main(void) {
	return EXIT_SUCCESS;
}

#endif
