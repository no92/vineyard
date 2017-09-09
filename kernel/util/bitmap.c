#include <driver/uart.h>
#include <util/bitmap.h>
#include <stdio.h>

#define BITS_PER_DWORD 32
#define LOG_BITS_PER_DWORD 5

void bitmap_set(bitmap_t *b, size_t i) {
	size_t offset = i % BITS_PER_DWORD;
	size_t entry = i >> LOG_BITS_PER_DWORD;

	b[entry] |= (1U << offset);
}

bool bitmap_get(bitmap_t *b, size_t i) {
	size_t offset = i % BITS_PER_DWORD;
	size_t entry = i >> LOG_BITS_PER_DWORD;

	return (b[entry] >> offset) & 0x01;
}

void bitmap_unset(bitmap_t *b, size_t i) {
	size_t offset = i % BITS_PER_DWORD;
	size_t entry = i >> LOG_BITS_PER_DWORD;

	b[entry] &= ~(1U << offset);
}

ssize_t bitmap_first_unset(bitmap_t *b) {
	ssize_t res = 0;
	bitmap_t *bitmap = b;

	if(!b) {
		return -1;
	}

	while(res < 0x20000) {
		if(*bitmap == 0xFFFFFFFF) {
			bitmap++;
			res++;
			continue;
		}

		return __builtin_ctzll(~(*bitmap)) + (res << 5);
	}

	return -1;
}
