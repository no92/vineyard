#include <ctype.h>

int ispunct(int c) {
	return isprint(c) && !isspace(c) && !isalnum(c);
}

#ifdef UNIT
#include <_libk_test.h>

int main(void) {
	return EXIT_SUCCESS;
}

#endif
