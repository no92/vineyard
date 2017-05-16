#include <stdbool.h>
#include <string.h>

size_t strspn(const char *s1, const char *s2) {
	size_t len1 = strlen(s1);
	size_t len2 = strlen(s2);

	for(size_t i = 0; i < len1; i++) {
		bool match = false;

		for(size_t j = 0; j < len2; j++) {
			if(s1[i] != s2[j]) {
				continue;
			}

			match = true;
			break;
		}

		if(!match) {
			return i;
		}
	}

	return len1;
}

#ifdef UNIT
#include <_libk_test.h>

int main(void) {
	return EXIT_SUCCESS;
}

#endif
