#pragma once

#ifdef __clang__
	#define A(x) __attribute__((annotate("oclint:suppress["x"]")))
#else
	#define A(x)
#endif
