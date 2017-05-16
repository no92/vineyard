#include <string.h>

size_t strxfrm(char * restrict s1, const char * restrict s2, size_t n) {
	size_t len = strlen(s2);

	if(n != 0) {
		strncpy(s1, s2, n);
	}

	return len;
}

#ifdef UNIT
#include <_libk_test.h>

int main(void) {
	/* TODO: implement this using malloc! */
	char *t1 = __builtin_alloca(4);

	assert(strxfrm(NULL, "0123456789", 0) == 10);
	assert(strxfrm(t1, "0123456789", 4) == 10);
	assert(!strcmp(t1, "0123"));
	assert(strcmp(t1, "01234") == -'4');

	return EXIT_SUCCESS;
}

#endif
