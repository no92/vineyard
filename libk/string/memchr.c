#include <string.h>

void *memchr(const void *s, int c, size_t n) {
	const uint8_t *buf = (const uint8_t *) s;

	for(size_t i = 0; i < n; i++) {
		if(buf[i] == (uint8_t) c) {
			return (void *) &buf[i];
		}
	}

	return NULL;
}

#ifdef UNIT
#include <_libk_test.h>

int main(void) {
	return EXIT_SUCCESS;
}

#endif
