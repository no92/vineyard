#include <ctype.h>

int tolower(int c) {
	if(isupper(c)) {
		return c + ('a' - 'A');
	}

	return c;
}

#ifdef UNIT
#include <_libk_test.h>

int main(void) {
	char t1[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

	for(size_t i = 0; i < strlen(t1); i++) {
		assert(tolower(t1[i]) == t1[i] + 32);
	}

	return EXIT_SUCCESS;
}

#endif
