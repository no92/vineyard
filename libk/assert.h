#pragma once

#include <init/panic.h>

#ifdef NDEBUG
	#define assert(x) ((void)0)
#else
	#define assert(x) ((x) ? ((void) 0) : panic("%s:%u (%s): %s failed\n", __FILE__, __LINE__, __func__, #x))
#endif

#define static_assert _Static_assert
