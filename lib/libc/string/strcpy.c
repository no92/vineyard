#include <string.h>

char *strcpy(char * restrict s1, const char * restrict s2) {
    char *s = s1;

    while(*s) {
		*s++ = *s2++;
	}

	return s1;
}

#ifdef UNIT
#include <_libk_test.h>

int main(void) {
	return EXIT_SUCCESS;
}

#endif
