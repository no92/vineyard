#pragma once

#ifdef __clang__
	#define A(x) __attribute__((annotate("oclint:suppress["x"]")))
	#define NO_UBSAN
#else
	#define A(x)
	#define NO_UBSAN __attribute__((no_sanitize_undefined))
#endif
