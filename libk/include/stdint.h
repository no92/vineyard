#pragma once

typedef signed char int8_t;
typedef signed short int16_t;
typedef signed int int32_t;

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;

typedef int8_t int_least8_t;
typedef int16_t int_least16_t;
typedef int32_t int_least32_t;

typedef uint8_t uint_least8_t;
typedef uint16_t uint_least16_t;
typedef uint32_t uint_least32_t;

typedef int8_t int_fast8_t;
typedef int16_t int_fast16_t;
typedef int32_t int_fast32_t;

typedef uint8_t uint_fast8_t;
typedef uint16_t uint_fast16_t;
typedef uint32_t uint_fast32_t;

#ifdef __i386__
	typedef signed long long int64_t;
	typedef unsigned long long uint64_t;

	typedef int32_t intptr_t;
	typedef uint32_t uintptr_t;
#elif __x86_64__
	typedef signed long int64_t;
	typedef unsigned long uint64_t;

	typedef int64_t intptr_t;
	typedef uint64_t uintptr_t;
#else
	#error Building for an unsupported arch. How did you do that?
#endif

typedef int64_t int_least64_t;
typedef uint64_t uint_least64_t;

typedef int64_t int_fast64_t;
typedef uint64_t uint_fast64_t;

typedef int64_t intmax_t;
typedef uint64_t uintmax_t;

#define INT8_C(c) c
#define INT16_C(c) c
#define INT32_C(c) c

#define UINT8_C(c) c##U
#define UINT16_C(c) c##U
#define UINT32_C(c) c##U

#ifdef __i386__
	#define INT64_C(c) c##LL
	#define UINT64_C(c) c##ULL

	#define INTMAX_C(c) c##LL
	#define UINTMAX_C(c) c##ULL
#elif __x86_64__
	#define INT64_C(c) c##L
	#define UINT64_C(c) c##UL

	#define INTMAX_C(c) c##L
	#define UINTMAX_C(c) c##UL
#else
	#error Building for an unsupported arch. How did you do that?
#endif

#define INT8_MIN (-128)
#define INT16_MIN (-32768)
#define INT32_MIN (-2147483648)
#define INT64_MIN (INT64_C(-9223372036854775808))

#define INT8_MAX (127)
#define INT16_MAX (32767)
#define INT32_MAX (2147483647)
#define INT64_MAX (INT64_C(9223372036854775807))

#define UINT8_MAX (255U)
#define UINT16_MAX (65535U)
#define UINT32_MAX (4294967295U)
#define UINT64_MAX (UINT64_C(18446744073709551615))

#define INT_LEAST8_MIN INT8_MIN
#define INT_LEAST16_MIN INT16_MIN
#define INT_LEAST32_MIN INT32_MIN
#define INT_LEAST64_MIN INT64_MIN

#define INT_LEAST8_MAX INT8_MAX
#define INT_LEAST16_MAX INT16_MAX
#define INT_LEAST32_MAX INT32_MAX
#define INT_LEAST64_MAX INT64_MAX

#define UINT_LEAST8_MAX UINT8_MAX
#define UINT_LEAST16_MAX UINT16_MAX
#define UINT_LEAST32_MAX UINT32_MAX
#define UINT_LEAST64_MAX UINT64_MAX

#define INT_FAST8_MIN INT8_MIN
#define INT_FAST16_MIN INT16_MIN
#define INT_FAST32_MIN INT32_MIN
#define INT_FAST64_MIN INT64_MIN

#define INT_FAST8_MAX INT8_MAX
#define INT_FAST16_MAX INT16_MAX
#define INT_FAST32_MAX INT32_MAX
#define INT_FAST64_MAX INT64_MAX

#define UINT_FAST8_MAX UINT8_MAX
#define UINT_FAST16_MAX UINT16_MAX
#define UINT_FAST32_MAX UINT32_MAX
#define UINT_FAST64_MAX UINT64_MAX

#ifdef __i386__
	#define INTPTR_MIN INT32_MIN
	#define INTPTR_MAX INT32_MAX
	#define UINTPTR_MAX UINT32_MAX
#elif __x86_64__
	#define INTPTR_MIN INT64_MIN
	#define INTPTR_MAX INT64_MAX
	#define UINTPTR_MAX UINT64_MAX
#else
	#error Building for an unsupported arch. How did you do that?
#endif

#define INTMAX_MIN INT64_MIN
#define INTMAX_MAX INT64_MAX
#define UINTMAX_MAX UINT64_MAX

#ifdef __i386__
	#define SIZE_MAX UINT32_MAX

	#define PTRDIFF_MIN INT32_MIN
	#define PTRDIFF_MAX INT32_MAX
#elif __x86_64__
	#define SIZE_MAX UINT64_MAX

	#define PTRDIFF_MIN INT64_MIN
	#define PTRDIFF_MAX INT64_MAX
#else
	#error Building for an unsupported arch. How did you do that?
#endif

/* TODO: SIG_ATOMIC_MIN */
/* TODO: SIG_ATOMIC_MAX */
/* TODO: WCHAR_MIN */
/* TODO: WCHAR_MAX */
/* TODO: WINT_MIN */
/* TODO: WINT_MAX */
