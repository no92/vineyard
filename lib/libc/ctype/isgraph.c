#include <ctype.h>

int isgraph(int c) {
	return c >= '!' && c <= '~';
}

#ifdef UNIT
#include <_libk_test.h>

int main(void) {
	return EXIT_SUCCESS;
}

#endif
