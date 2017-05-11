#include <ctype.h>

int isalnum(int c) {
	return isalpha(c) || isdigit(c);
}

#ifdef UNIT
#include <_libk_test.h>

int main(void) {
	return EXIT_SUCCESS;
}

#endif
