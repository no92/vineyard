#include <stdbool.h>
#include <utf8.h>

/* check wheter a char is an UTF-8 continuation byte */
bool u8cont(const char b) {
	return (b & 0xC0) == 0x80;
}

#ifdef UNIT
#include <_libk_test.h>

int main(void) {
	return EXIT_SUCCESS;
}

#endif
