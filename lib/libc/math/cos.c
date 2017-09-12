#include <math.h>

double cos(double x) {
	return __builtin_cos(x);
}

float cosf(float x) {
	return __builtin_cosf(x);
}

long double cosl(long double x) {
	return __builtin_cosl(x);
}

#ifdef UNIT
#include <_libk_test.h>

int main(void) {
	return EXIT_SUCCESS;
}

#endif
