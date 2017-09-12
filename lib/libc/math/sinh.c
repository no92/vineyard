#include <math.h>

double sinh(double x) {
	return __builtin_sinh(x);
}

float sinhf(float x) {
	return __builtin_sinhf(x);
}

long double sinhl(long double x) {
	return __builtin_sinhl(x);
}

#ifdef UNIT
#include <_libk_test.h>

int main(void) {
	return EXIT_SUCCESS;
}

#endif
