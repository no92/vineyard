#pragma once

#include <stdint.h>

#ifdef __i386__
	#define __PRI64_PREFIX	"ll"
	#define __PRIPOINTER_PREFIX	""
#elif __x86_64__
	#define __PRI64_PREFIX	"l"
	#define __PRIPOINTER_PREFIX	"l"
#else
	#error Building for an unsupported arch. How did you do that?
#endif

#define PRId8			"d"
#define PRId16			"d"
#define PRId32			"d"
#define PRId64			__PRI64_PREFIX"d"

#define PRIdLEAST8		"d"
#define PRIdLEAST16		"d"
#define PRIdLEAST32		"d"
#define PRIdLEAST64		__PRI64_PREFIX"d"

#define PRIdFAST8		"d"
#define PRIdFAST16		"d"
#define PRIdFAST32		"d"
#define PRIdFAST64		__PRI64_PREFIX"d"

#define PRIdMAX			__PRI64_PREFIX"d"
#define PRIdPTR			__PRIPOINTER_PREFIX"d"

#define PRIi8			"i"
#define PRIi16			"i"
#define PRIi32			"i"
#define PRIi64			__PRI64_PREFIX"i"

#define PRIiLEAST8		"i"
#define PRIiLEAST16		"i"
#define PRIiLEAST32		"i"
#define PRIiLEAST64		__PRI64_PREFIX"i"

#define PRIiFAST8		"i"
#define PRIiFAST16		"i"
#define PRIiFAST32		"i"
#define PRIiFAST64		__PRI64_PREFIX"i"

#define PRIiMAX			__PRI64_PREFIX"i"
#define PRIiPTR			__PRIPOINTER_PREFIX"i"

#define PRIu8			"u"
#define PRIu16			"u"
#define PRIu32			"u"
#define PRIu64			__PRI64_PREFIX"u"

#define PRIuLEAST8		"u"
#define PRIuLEAST16		"u"
#define PRIuLEAST32		"u"
#define PRIuLEAST64		__PRI64_PREFIX"u"

#define PRIuFAST8		"u"
#define PRIuFAST16		"u"
#define PRIuFAST32		"u"
#define PRIuFAST64		__PRI64_PREFIX"u"

#define PRIuMAX			__PRI64_PREFIX"u"
#define PRIuPTR			__PRIPOINTER_PREFIX"u"

#define PRIx8			"x"
#define PRIx16			"x"
#define PRIx32			"x"
#define PRIx64			__PRI64_PREFIX"x"

#define PRIxLEAST8		"x"
#define PRIxLEAST16		"x"
#define PRIxLEAST32		"x"
#define PRIxLEAST64		__PRI64_PREFIX"x"

#define PRIxFAST8		"x"
#define PRIxFAST16		"x"
#define PRIxFAST32		"x"
#define PRIxFAST64		__PRI64_PREFIX"x"

#define PRIxMAX			__PRI64_PREFIX"x"
#define PRIxPTR			__PRIPOINTER_PREFIX"x"

#define PRIX8			"X"
#define PRIX16			"X"
#define PRIX32			"X"
#define PRIX64			__PRI64_PREFIX"X"

#define PRIXLEAST8		"X"
#define PRIXLEAST16		"X"
#define PRIXLEAST32		"X"
#define PRIXLEAST64		__PRI64_PREFIX"X"

#define PRIXFAST8		"X"
#define PRIXFAST16		"X"
#define PRIXFAST32		"X"
#define PRIXFAST64		__PRI64_PREFIX"X"

#define PRIXMAX			__PRI64_PREFIX"X"
#define PRIXPTR			__PRIPOINTER_PREFIX"X"

#define PRIo8			"o"
#define PRIo16			"o"
#define PRIo32			"o"
#define PRIo64			__PRI64_PREFIX"o"

#define PRIoLEAST8		"o"
#define PRIoLEAST16		"o"
#define PRIoLEAST32		"o"
#define PRIoLEAST64		__PRI64_PREFIX"o"

#define PRIoFAST8		"o"
#define PRIoFAST16		"o"
#define PRIoFAST32		"o"
#define PRIoFAST64		__PRI64_PREFIX"o"

#define PRIoMAX			__PRI64_PREFIX"o"
#define PRIoPTR			__PRIPOINTER_PREFIX"o"
