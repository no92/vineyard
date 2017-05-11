#include <ctype.h>

int isalpha(int c) {
	return islower(c) || isupper(c);
}

#ifdef UNIT
#include <_libk_test.h>

int main(void) {
	return EXIT_SUCCESS;
}

#endif
