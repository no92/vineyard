#pragma once

#include <stdnoreturn.h>

noreturn __attribute__((format(printf, 1, 2))) void panic(const char * restrict format, ...);
