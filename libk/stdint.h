#pragma once

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;

typedef signed char int8_t;
typedef signed short int16_t;
typedef signed int int32_t;

#ifdef __i386__
	typedef uint32_t uintptr_t;
#elif __x86_64__
	typedef unsigned long uint64_t;
	typedef signed long int64_t;

	typedef uint64_t uintptr_t;
#else
	#error Building for an unsupported arch. How did you do that?
#endif
