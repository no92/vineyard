#include <strings.h>

int strncasecmp(const char *s1, const char *s2, size_t n) {
	return __builtin_strncasecmp(s1, s2, n);
}

#ifdef UNIT
#include <_libk_test.h>

int main(void) {
	return EXIT_SUCCESS;
}

#endif
