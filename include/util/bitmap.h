#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <sys/types.h>

typedef uint32_t bitmap_t;

void bitmap_set(bitmap_t *b, size_t i);
bool bitmap_get(bitmap_t *b, size_t i);
void bitmap_unset(bitmap_t *b, size_t i);
ssize_t bitmap_first_unset(bitmap_t *b);
