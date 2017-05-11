#include <string.h>

char *strncat(char * restrict s1, const char * restrict s2, size_t n) {
	strncpy(s1 + strlen(s1), s2, n);
	*(s1 + strlen(s1) + n) = '\0';

	return s1;
}

#ifdef UNIT
#include <_libk_test.h>

int main(void) {
	return EXIT_SUCCESS;
}

#endif
