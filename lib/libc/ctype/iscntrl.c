#include <ctype.h>

int iscntrl(int c) {
	return c > 0 && c < 32;
}

#ifdef UNIT
#include <_libk_test.h>

int main(void) {
	assert(iscntrl('\033'));

	return EXIT_SUCCESS;
}

#endif
