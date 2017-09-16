#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

bool u8cont(const char b);
__attribute__((pure, nonnull(1))) size_t u8len(const char *c);
__attribute__((pure, nonnull(1))) size_t u8strlen(const char *str);