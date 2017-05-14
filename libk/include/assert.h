#pragma once

#ifdef __libk
	#include <init/panic.h>
#else
	extern void __assert (const char *msg, const char *file, int line);
#endif

#ifdef NDEBUG
	#define assert(x) ((void)0)
#else
	#ifdef __libk
		#define assert(x) ((x) ? ((void) 0) : panic("%s:%u (%s): %s failed\n", __FILE__, __LINE__, __func__, #x))
	#else
		#define assert(x) ((x) ? ((void) 0) : __assert(#x, __FILE__, __LINE__))
	#endif
#endif

#define static_assert _Static_assert
