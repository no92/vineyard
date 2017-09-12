#include <ctype.h>

int isdigit(int c) {
	return c >= '0' && c <= '9';
}

#ifdef UNIT
#include <_libk_test.h>

int main(void) {
	char t1[] = "0123456789";

	for(size_t i = 0; i < strlen(t1); i++) {
		assert(isdigit(t1[i]));
	}

	return EXIT_SUCCESS;
}

#endif
