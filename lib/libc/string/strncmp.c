#include <string.h>

int strncmp(const char *s1, const char *s2, size_t n) {
	const unsigned char *str1 = (const unsigned char *) s1;
	const unsigned char *str2 = (const unsigned char *) s2;

	while(--n > 0 && *str1 && *str1 == *str2) {
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
	const char *t4 = "עברית ";

	assert(!strncmp(t1, t2, 4));
	assert(strncmp(t1, t2, 5) == -21);
	assert(!strncmp(t3, t4, 10));
	assert(strncmp(t3, t4, 11) == -32);

	return EXIT_SUCCESS;
}

#endif
