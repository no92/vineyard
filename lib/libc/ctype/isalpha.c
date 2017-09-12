#include <ctype.h>

int isalpha(int c) {
	return islower(c) || isupper(c);
}

#ifdef UNIT
#include <_libk_test.h>

int main(void) {
	char t1[] = "abcdefghijklmnopqrstuvwxyz";

	for(size_t i = 0; i < strlen(t1); i++) {
		assert(isalpha(t1[i]));
	}

	return EXIT_SUCCESS;
}

#endif
