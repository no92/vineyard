#include <ctype.h>

int tolower(int c) {
	if(isupper(c)) {
		return c + ('a' - 'A');
	}

	return c;
}

#ifdef UNIT
#include <_libk_test.h>

int main(void) {
	return EXIT_SUCCESS;
}

#endif
