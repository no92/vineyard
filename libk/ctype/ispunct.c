#include <ctype.h>

int ispunct(int c) {
	return isprint(c) && !isspace(c) && !isalnum(c);
}
