#include <string.h>
#include <stdlib.h>

char *strdup(const char *s1) {
	size_t s = strlen(s1);
	char *ret = (char *) malloc(s + 1);

	if(!ret) {
		return NULL;
	}

	memcpy(ret, s1, s + 1);

	return ret;
}
