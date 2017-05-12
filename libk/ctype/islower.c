#include <ctype.h>

int islower(int c) {
	return (c >= 'a' && c <= 'z');
}

#ifdef UNIT
#include <_libk_test.h>

int main(void) {
	char t1[] = "abcdefghijklmnopqrstuvwxyz";

	for(size_t i = 0; i < strlen(t1); i++) {
		assert(islower(t1[i]));
	}

	return EXIT_SUCCESS;
}

#endif
