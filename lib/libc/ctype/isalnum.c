#include <ctype.h>

int isalnum(int c) {
	return isalpha(c) || isdigit(c);
}

#ifdef UNIT
#include <_libk_test.h>

int main(void) {
	char t1[] = "abcdefghijklmnopqrstuvwxyz0123456789";

	for(size_t i = 0; i < strlen(t1); i++) {
		assert(isalnum(t1[i]));
	}

	return EXIT_SUCCESS;
}

#endif
