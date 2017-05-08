#include <stdint.h>
#include <utf8.h>

size_t u8len(const char *c) {
	if((c[0] & 0x80) == 0) {
		/* single-byte */
		return 1;
	} else if((c[0] & 0xE0) == 0xC0 && u8cont(c[1])) {
		/* 2 bytes */
		return 2;
	} else if((c[0] & 0xF0) == 0xE0 && u8cont(c[1]) && u8cont(c[2])) {
		/* 3 bytes */
		return 3;
	} else if((c[0] & 0xF8) == 0xF0 && u8cont(c[1]) && u8cont(c[2]) && u8cont(c[3])) {
		/* 4 bytes */
		return 4;
	}

	return 0;
}
