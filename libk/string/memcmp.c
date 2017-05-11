#include <string.h>

int memcmp(const void *s1, const void *s2, size_t n) {
	const uint8_t *m1 = (const uint8_t *) s1;
	const uint8_t *m2 = (const uint8_t *) s2;

	while(n && *m1 == *m2) {
		m1++;
		m2++;
		n--;
	}

	return (n == 0) ? 0 : (*m1 - *m2);
}

#ifdef UNIT
#include <_libk_test.h>

int main(void) {
	const char *t1 = "12345";
	const char *t2 = "12340";

	assert(memcmp(t1, t1, 5) == 0);
	assert(memcmp(t2, t2, 5) == 0);
	assert(memcmp(t1, t2, 4) == 0);
	assert(memcmp(t1, t2, 5) == 5);
	assert(memcmp(t2, t1, 5) == -5);

	return EXIT_SUCCESS;
}

#endif
