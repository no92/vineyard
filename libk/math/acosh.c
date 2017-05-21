#include <math.h>

double acosh(double x) {
	return __builtin_acosh(x);
}

float acoshf(float x) {
	return __builtin_acoshf(x);
}

long double acoshl(long double x) {
	return __builtin_acoshl(x);
}

#ifdef UNIT
#include <_libk_test.h>

int main(void) {
	return EXIT_SUCCESS;
}

#endif
