#include <string.h>

void *memchr(const void *s, int c, size_t n) {
	const uint8_t *buf = (const uint8_t *) s;

	while(n --> 0) {
		if(*buf == (uint8_t) c) {
			return (void *) buf;
		}

		buf++;
	}

	return NULL;
}

#ifdef UNIT
#include <_libk_test.h>

int main(void) {
	/* char *t1 = "lorem ipsum" */
	const char *t1 = "\x6c\x6f\x72\x65\x6d\x20\x69\x70\x73\x75\x6d";

	assert(memchr(t1, 'a', 12) == NULL);
	assert(memchr(t1, ' ', 6) == t1 + 5);
	assert(memchr(t1, ' ', 5) == NULL);

	return EXIT_SUCCESS;
}

#endif
