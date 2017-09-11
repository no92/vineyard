#pragma once

#include <stdnoreturn.h>

__attribute__((format(printf, 1, 2))) noreturn void die(const char *format, ...);
