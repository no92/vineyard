#pragma once

typedef signed char __int8_t;
typedef signed short __int16_t;
typedef signed int __int32_t;

typedef unsigned char __uint8_t;
typedef unsigned short __uint16_t;
typedef unsigned int __uint32_t;

typedef __int8_t __int_least8_t;
typedef __int16_t __int_least16_t;
typedef __int32_t __int_least32_t;

typedef __uint8_t __uint_least8_t;
typedef __uint16_t __uint_least16_t;
typedef __uint32_t __uint_least32_t;

typedef __int8_t __int_fast8_t;
typedef __int16_t __int_fast16_t;
typedef __int32_t __int_fast32_t;

typedef __uint8_t __uint_fast8_t;
typedef __uint16_t __uint_fast16_t;
typedef __uint32_t __uint_fast32_t;

#ifdef __i386__
	typedef signed long long __int64_t;
	typedef unsigned long long __uint64_t;

	typedef __int32_t __intptr_t;
	typedef __uint32_t __uintptr_t;
#elif __x86_64__
	typedef signed long __int64_t;
	typedef unsigned long __uint64_t;

	typedef __int64_t __intptr_t;
	typedef __uint64_t __uintptr_t;
#else
	#error Building for an unsupported arch. How did you do that?
#endif

typedef __int64_t __int_least64_t;
typedef __uint64_t __uint_least64_t;

typedef __int64_t __int_fast64_t;
typedef __uint64_t __uint_fast64_t;

typedef __int64_t __intmax_t;
typedef __uint64_t __uintmax_t;

#ifdef __i386__
	typedef __int32_t __ptrdiff_t;
	typedef __uint32_t __size_t;
	typedef __int32_t __ssize_t;
#elif __x86_64__
	typedef __int64_t __ptrdiff_t;
	typedef __uint64_t __size_t;
	typedef __int64_t __ssize_t;
#else
	#error Building for an unsupported arch. How did you do that?
#endif

typedef __size_t __pid_t;

typedef struct {
	int fd;
} __FILE;
