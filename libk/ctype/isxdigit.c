#include <ctype.h>

int isxdigit(int c) {
	return isdigit(c) || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F');
}

#ifdef UNIT
#include <_libk_test.h>

int main(void) {
	char t1[] = "0123456789abcdefABCDEF";

	for(size_t i = 0; i < strlen(t1); i++) {
		assert(isxdigit(t1[i]));
	}

	return EXIT_SUCCESS;
}

#endif
