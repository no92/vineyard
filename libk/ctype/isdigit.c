#include <ctype.h>

int isdigit(int c) {
	return (c >= '0' && c <= '9');
}

#ifdef UNIT
#include <_libk_test.h>

int main(void) {
	return EXIT_SUCCESS;
}

#endif
