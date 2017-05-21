#include <math.h>

double atanh(double x) {
	return __builtin_atanh(x);
}

float atanhf(float x) {
	return __builtin_atanhf(x);
}

long double atanhl(long double x) {
	return __builtin_atanhl(x);
}

#ifdef UNIT
#include <_libk_test.h>

int main(void) {
	return EXIT_SUCCESS;
}

#endif
