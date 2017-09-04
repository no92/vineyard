#pragma once

#ifdef __clang__
	#define A(x) __attribute__((annotate("oclint:suppress["x"]")))
	#define NO_UBSAN
#else
	#define A(x)
	#define NO_UBSAN __attribute__((no_sanitize_undefined))
#endif

#define ALIGN_UP(addr, alignment)	((addr + (alignment - 1U)) & ~(alignment - 1U))

#define container_of(ptr, type, member) \
                      ((type *) ((char *)(ptr) - offsetof(type, member)))
