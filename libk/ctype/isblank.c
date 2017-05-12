#include <ctype.h>

int isblank(int c) {
	return c == ' ' || c == '\t';
}

#ifdef UNIT
#include <_libk_test.h>

int main(void) {
	assert(isblank(' '));
	assert(isblank('	'));

	return EXIT_SUCCESS;
}

#endif
