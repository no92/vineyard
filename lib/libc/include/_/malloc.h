#ifdef __vineyard__

#pragma once

#include <_/dlmalloc.h>
#include <die.h>

#define ABORT die("dlmalloc called abort()")
#define ABORT_ON_ASSERT_FAILURE 0

#define LACKS_FCNTL_H
#define LACKS_TIME_H
#define LACKS_UNISTD_H
#define LACKS_SYS_PARAM_H

#define NO_MALLOC_STATS 1
#define MALLOC_FAILURE_ACTION

#define HAVE_MORECORE 0
#define HAVE_MMAP 1

#define USE_LOCKS 0
#define USE_SPIN_LOCKS 0

#define malloc_getpagesize 0x1000

#endif
