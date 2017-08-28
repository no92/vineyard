#pragma once

#include <stdint.h>

#ifdef __i386__
	typedef int32_t ssize_t;
	typedef uint32_t size_t;
#elif __x86_64__
	typedef int64_t ssize_t;
	typedef uint64_t size_t;
#else
	#error Building for an unsupported arch. How did you do that?
#endif

typedef long int off_t;
