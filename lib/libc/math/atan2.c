#include <math.h>

double atan2(double y, double x) {
	return __builtin_atan2(y, x);
}

float atan2f(float y, float x) {
	return __builtin_atan2f(y, x);
}

long double atan2l(long double y, long double x) {
	return __builtin_atan2l(y, x);
}

#ifdef UNIT
#include <_libk_test.h>

int main(void) {
	return EXIT_SUCCESS;
}

#endif
