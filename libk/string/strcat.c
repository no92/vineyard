#include <string.h>

char *strcat(char * restrict s1, const char * restrict s2) {
	strcpy(s1 + strlen(s1), s2);

	return s1;
}

#ifdef UNIT
#include <_libk_test.h>

int main(void) {
	return EXIT_SUCCESS;
}

#endif
