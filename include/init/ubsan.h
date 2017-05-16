#pragma once

#include <stdint.h>
#include <stdnoreturn.h>

#define is_aligned(value, alignment) !(value & (alignment - 1))

typedef struct {
	const char *file;
	uint32_t line;
	uint32_t column;
} ubsan_location_t;

typedef struct {
	uint16_t kind;
	uint16_t info;
	char name[];
} ubsan_type_t;

typedef struct {
	ubsan_location_t location;
	ubsan_type_t *type;
	uintptr_t alignment;
	uint8_t type_check_kind;
} ubsan_type_mismatch_t;

const char *type_check_kinds[] = {
	"load of",
	"store to",
	"reference binding to",
	"member access within",
	"member call on",
	"constructor call on",
	"downcast of",
	"downcast of",
	"upcast of",
	"cast to virtual base of",
};

typedef struct {
    ubsan_location_t location;
    const ubsan_type_t *left_type;
    const ubsan_type_t *right_type;
} ubsan_out_of_bounds_t;

typedef struct {
	ubsan_location_t loc;
	ubsan_location_t attr_loc;
	int argument;
} ubsan_nonnull_arg_t;

noreturn void __ubsan_handle_type_mismatch(ubsan_type_mismatch_t *type_mismatch, uintptr_t pointer);
noreturn void __ubsan_handle_out_of_bounds(ubsan_out_of_bounds_t *out_of_bounds, uintptr_t pointer);
noreturn void __ubsan_handle_negate_overflow(ubsan_location_t *location, uintptr_t pointer);
noreturn void __ubsan_handle_sub_overflow(ubsan_location_t *location, uintptr_t left, uintptr_t right);
noreturn void __ubsan_handle_add_overflow(ubsan_location_t *location, uintptr_t left, uintptr_t right);
noreturn void __ubsan_handle_mul_overflow(ubsan_location_t *location, uintptr_t left, uintptr_t right);
noreturn void __ubsan_handle_divrem_overflow(ubsan_location_t *location, uintptr_t left, uintptr_t right);
noreturn void __ubsan_handle_shift_out_of_bounds(ubsan_location_t *location, uintptr_t left, uintptr_t right);
noreturn void __ubsan_handle_nonnull_arg(ubsan_nonnull_arg_t *data);
noreturn void __ubsan_handle_builtin_unreachable(ubsan_location_t *location);
