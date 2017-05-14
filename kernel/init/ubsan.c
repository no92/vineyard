#include <init/ubsan.h>
#include <init/panic.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdnoreturn.h>

/* TODO: move this to libk? */

static inline void ubsan_location(ubsan_location_t *location) {
	printf("[ubsan]	caught undefined behavior in %s:%u:%u\n", location->file, location->line, location->column);
}

noreturn void __ubsan_handle_type_mismatch(ubsan_type_mismatch_t *type_mismatch, uintptr_t pointer) {
	ubsan_location(&type_mismatch->location);

	if(pointer == 0) {
		panic("[ubsan]	null pointer access");
	} else if(type_mismatch->alignment != 0 && is_aligned(pointer, type_mismatch->alignment)) {
		panic("[ubsan]	unaligned memory access");
	} else {
		panic("[ubsan]	%s adress %p with insufficient space for object of type %s\n", type_check_kinds[type_mismatch->type_check_kind], (void *) pointer, type_mismatch->type->name);
	}

	__builtin_unreachable();
}

noreturn void __ubsan_handle_out_of_bounds(ubsan_out_of_bounds_t *out_of_bounds, uintptr_t index) {
	ubsan_location(&out_of_bounds->location);

	panic("[ubsan]	index %d is out of bounds", index);

	__builtin_unreachable();
}

noreturn void __ubsan_handle_negate_overflow(ubsan_location_t *location, uintptr_t value) {
	ubsan_location(location);

	panic("[ubsan]	%u; negation overflow", value);

	__builtin_unreachable();
}

noreturn void __ubsan_handle_sub_overflow(ubsan_location_t *location, uintptr_t left, uintptr_t right) {
	ubsan_location(location);

	panic("[ubsan]	%d - %d; subtraction overflow", left, right);

	__builtin_unreachable();
}

noreturn void __ubsan_handle_add_overflow(ubsan_location_t *location, uintptr_t left, uintptr_t right) {
	ubsan_location(location);

	panic("[ubsan]	%d + %d; addition overflow", left, right);

	__builtin_unreachable();
}

noreturn void __ubsan_handle_mul_overflow(ubsan_location_t *location, uintptr_t left, uintptr_t right) {
	ubsan_location(location);

	panic("[ubsan]	%d * %d; multiplication overflow", left, right);

	__builtin_unreachable();
}

noreturn void __ubsan_handle_divrem_overflow(ubsan_location_t *location, uintptr_t left, uintptr_t right) {
	ubsan_location(location);

	panic("[ubsan]	%d divrem %d; divrem overflow", left, right);

	__builtin_unreachable();
}

noreturn void __ubsan_handle_shift_out_of_bounds(ubsan_location_t *location, uintptr_t left, uintptr_t right) {
	ubsan_location(location);

	panic("[ubsan]	shift %d by %d; shift out of bounds", left, right);

	__builtin_unreachable();
}

noreturn void __ubsan_handle_builtin_unreachable(ubsan_location_t *location) {
	ubsan_location(location);

	panic("[ubsan]	called __builtin_unreachable()");
}
