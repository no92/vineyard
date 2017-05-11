#include <stdio.h>
#include <string.h>

int strcmp(const char *s1, const char *s2) {
	const unsigned char *str1 = (const unsigned char *) s1;
	const unsigned char *str2 = (const unsigned char *) s2;

	while(*str1 && *str1 == *str2) {
		str1++;
		str2++;
	}

	return *str1 - *str2;
}

#ifdef UNIT
#include <_libk_test.h>

int main(void) {
	const char *t1 = "abcde";
	const char *t2 = "abcdz";
	const char *t3 = "עברית";
	const char *t4 = "עברית";

	assert(!strcmp(t3, t4));
	assert(strcmp(t1, t2) == -21);
	assert(strcmp(t2, t1) == 21);
	assert(!strcmp(t2, t2));

	return EXIT_SUCCESS;
}

#endif
