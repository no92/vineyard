#include <math.h>

double sin(double x) {
	return __builtin_sin(x);
}

float sinf(float x) {
	return __builtin_sinf(x);
}

long double sinl(long double x) {
	return __builtin_sinl(x);
}

#ifdef UNIT
#include <_libk_test.h>

int main(void) {
	return EXIT_SUCCESS;
}

#endif
