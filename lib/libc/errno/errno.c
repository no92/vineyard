#include <errno.h>

int errno = 0;

#ifdef UNIT
#include <_libk_test.h>

int main(void) {
	return EXIT_SUCCESS;
}
#endif
