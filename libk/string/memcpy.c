#include <stddef.h>
#include <string.h>

/* TODO: this implementation is naive, improve this! */
void *memcpy(void * restrict s1, const void * restrict s2, size_t n) {
	char *dst = (char *) s1;
	const char *src = (char *) s2;

	while(n--) {
		*dst++ = *src++;
	}

	return s1;
}

#ifdef UNIT
#include <_libk_test.h>

int main(void) {
	char buf[16];
	char *t1 = "test string";

	memcpy(buf, t1, strlen(t1));
	buf[strlen(t1)] = '\0';

	assert(strlen(buf) == strlen(t1));
	assert(!strcmp(buf, t1));

	return EXIT_SUCCESS;
}

#endif
