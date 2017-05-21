#include <math.h>

double acos(double x) {
	return __builtin_acos(x);
}

float acosf(float x) {
	return __builtin_acosf(x);
}

long double acosl(long double x) {
	return __builtin_acosl(x);
}

#ifdef UNIT
#include <_libk_test.h>

int main(void) {
	return EXIT_SUCCESS;
}

#endif
