#include <stdint.h>
#include <string.h>

void *memmove(void *s1, const void *s2, size_t n) {
	if(s2 == s1) {
		return s1;
	}

	const void *src_end = (const void *) ((uintptr_t) s2 + n);

	/* TODO: aligned 32-bit copy */
	if(s2 < s1 && s1 < src_end) {
		char *dst8 = (char *) s1;
		const char *src8 = (const char *) s2;

		for(ptrdiff_t i = (int32_t) n - 1; i >= 0; i--) {
			dst8[i] = src8[i];
		}

		return s1;
	}

	return memcpy(s1, s2, n);
}

#ifdef UNIT
#include <_libk_test.h>

int main(void) {
	char t1[] = "     abcde";

	memmove(t1, t1 + 5, 3);
	assert(!strcmp("abc  abcde", t1));
	memmove(t1, t1 + 1, 4);
	assert(!strcmp("bc   abcde", t1));

	return EXIT_SUCCESS;
}

#endif