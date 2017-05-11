#include <ctype.h>

int isupper(int c) {
	return (c >= 'A' && c <= 'Z');
}

#ifdef UNIT
#include <_libk_test.h>

int main(void) {
	return EXIT_SUCCESS;
}

#endif
