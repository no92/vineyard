#include <math.h>

double cosh(double x) {
	return __builtin_cosh(x);
}

float coshf(float x) {
	return __builtin_coshf(x);
}

long double coshl(long double x) {
	return __builtin_coshl(x);
}

#ifdef UNIT
#include <_libk_test.h>

int main(void) {
	return EXIT_SUCCESS;
}

#endif
