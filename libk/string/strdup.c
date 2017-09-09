#include <string.h>
#include <stdlib.h>

char *strdup(const char *s1) {
#ifdef __libk
	size_t s = strlen(s1);
	char *ret = (char *) malloc(s + 1);

	if(!ret) {
		return NULL;
	}

	memcpy(ret, s1, s + 1);

	return ret;
#else
	(void) s1;

	return NULL;
#endif
}

#ifdef UNIT
#include <_libk_test.h>

int main(void) {
	return EXIT_SUCCESS;
}

#endif
