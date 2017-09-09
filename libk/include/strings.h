#pragma once

#include <stddef.h>

int ffs(int i);
__attribute__((pure)) int strcasecmp(const char *s1, const char *s2);
__attribute__((pure)) int strncasecmp(const char *s1, const char *s2, size_t n);
