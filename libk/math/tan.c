#include <math.h>

double tan(double x) {
	return __builtin_tan(x);
}

float tanf(float x) {
	return __builtin_tanf(x);
}

long double tanl(long double x) {
	return __builtin_tanl(x);
}

#ifdef UNIT
#include <_libk_test.h>

int main(void) {
	return EXIT_SUCCESS;
}

#endif
