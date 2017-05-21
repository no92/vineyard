#include <strings.h>

int ffs(int i) {
	return __builtin_ffs(i);
}

#ifdef UNIT
#include <_libk_test.h>

int main(void) {
	return EXIT_SUCCESS;
}

#endif
