#include <string.h>

char *strncpy(char * restrict s1, const char * restrict s2, size_t n) {
    char *s = s1;

    while((n > 0) && (*s++ = *s2++)) {
		n--;
	}

	while(n --> 1) {
		*s++ = '\0';
	}

	return s1;
}

#ifdef UNIT
#include <_libk_test.h>

int main(void) {
	return EXIT_SUCCESS;
}

#endif
