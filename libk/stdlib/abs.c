#include <stdlib.h>

int abs(int j) {
	return __builtin_abs(j);
}

long labs(long j) {
	return __builtin_labs(j);
}

long long llabs(long long j) {
	return __builtin_llabs(j);
}
