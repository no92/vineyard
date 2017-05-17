#pragma once

#include <init/multiboot2.h>

#include <stddef.h>

void trace_init(multiboot2_t *multiboot);
const char *trace_lookup_addr(uintptr_t addr);
uintptr_t trace_lookup_name(const char *name);
void trace(size_t levels);
