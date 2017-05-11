#include <ctype.h>

int toupper(int c) {
	if(islower(c)) {
		return c - ('a' - 'A');
	}

	return c;
}

#ifdef UNIT
#include <_libk_test.h>

int main(void) {
	return EXIT_SUCCESS;
}

#endif
