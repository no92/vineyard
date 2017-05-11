#include <ctype.h>

int islower(int c) {
	return (c >= 'a' && c <= 'z');
}

#ifdef UNIT
#include <_libk_test.h>

int main(void) {
	return EXIT_SUCCESS;
}

#endif
