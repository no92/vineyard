#include <stddef.h>
#include <utf8.h>

/* a strlen(3) implementation for UTF-8 strings */
size_t u8strlen(const char *str) {
	size_t i = 0;
	size_t j = 0;

	while(str[i]) {
		if((str[i] & 0xC0) != 0x80) {
			j++;
		}

		i++;
	}

	return j;
}

#ifdef UNIT
#include <_libk_test.h>

int main(void) {
	return EXIT_SUCCESS;
}

#endif
