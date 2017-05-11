#include <ctype.h>

int isspace(int c) {
	return c == ' ' || c == '\f' || c == '\n' || c == '\r' || c == '\t' || c == '\v';
}

#ifdef UNIT
#include <_libk_test.h>

int main(void) {
	return EXIT_SUCCESS;
}

#endif
