#include <ctype.h>

int isxdigit(int c) {
	return isdigit(c) || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F');
}

#ifdef UNIT
#include <_libk_test.h>

int main(void) {
	return EXIT_SUCCESS;
}

#endif
