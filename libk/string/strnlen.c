#include <_/vineyard.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

/* GNU extension */
A("empty for statement")
size_t strnlen(const char *s, size_t maxlen) {
	const char *str;

	for(str = s; *str && maxlen > 0; str++, maxlen--);

	return (size_t) (str - s);
}

#ifdef UNIT
#include <_libk_test.h>

int main(void) {
	const char *t1 = "test";
	const char *t2 = "longer string";
	const char *t3 = "עברית";

	assert(strnlen(t1, 4) == 4);
	assert(strnlen(t2, 13) == 13);
	assert(strnlen(t3, 10) == 10);
	assert(strnlen(t1, 3) == 3);
	assert(strnlen(t3, 8) == 8);
	assert(strnlen(t3, 12) == 10);

	return EXIT_SUCCESS;
}

#endif
