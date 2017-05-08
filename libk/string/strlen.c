#include <stddef.h>
#include <string.h>
#include <stdio.h>

size_t strlen(const char *s) {
	const char *str;

	for(str = s; *str; str++);

	return (size_t) (str - s);
}
