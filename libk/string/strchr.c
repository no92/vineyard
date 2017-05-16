#include <string.h>

char *strchr(const char *s, int c) {
	if((uint8_t) c == '\0') {
		return (char *) s + strlen(s);
	}

	while(*s) {
		if(*s == (uint8_t) c) {
			return (char *) s;
		}

		s++;
	}

	return NULL;
}

#ifdef UNIT
#include <_libk_test.h>

int main(void) {
	const char *t1 = "test string";

	assert(strchr(t1, ' ') == t1 + 4);
	assert(strchr(t1, '\0') == t1 + 11);

	return EXIT_SUCCESS;
}

#endif
