#pragma once

#include <_/types.h>

#ifndef __PID_T
#define __PID_T
typedef __pid_t pid_t;
#endif

pid_t getpid(void);
