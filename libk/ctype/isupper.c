#include <ctype.h>

int isupper(int c) {
	return (c >= 'A' && c <= 'Z');
}

#ifdef UNIT
#include <_libk_test.h>

int main(void) {
	char t1[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

	for(size_t i = 0; i < strlen(t1); i++) {
		assert(isupper(t1[i]));
	}

	return EXIT_SUCCESS;
}

#endif
