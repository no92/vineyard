#include <ctype.h>

int isprint(int c) {
	return isgraph(c) || c == ' ';
}

#ifdef UNIT
#include <_libk_test.h>

int main(void) {
	return EXIT_SUCCESS;
}

#endif
