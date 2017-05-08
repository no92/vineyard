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
