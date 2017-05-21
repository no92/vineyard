#include <strings.h>

int strcasecmp(const char *s1, const char *s2) {
	return __builtin_strcasecmp(s1, s2);
}

#ifdef UNIT
#include <_libk_test.h>

int main(void) {
	return EXIT_SUCCESS;
}

#endif
