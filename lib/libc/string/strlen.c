#include <_/vineyard.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

size_t strlen(const char *s) {
	const char *str;

	for(str = s; *str; str++);

	return (size_t) (str - s);
}

#ifdef UNIT
#include <_libk_test.h>

int main(void) {
	const char *t1 = "test";
	const char *t2 = "longer string";
	const char *t3 = "עברית";

	assert(strlen(t1) == 4);
	assert(strlen(t2) == 13);
	assert(strlen(t3) == 10);

	return EXIT_SUCCESS;
}

#endif
