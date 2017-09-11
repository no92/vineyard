#include <die.h>
#include <stdio.h>
#include <ubsan.h>

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

static void ubsan_location(ubsan_location_t *location) {
	if(location->file) {
		printf("[ubsan]	caught undefined behavior in %s:%u:%u\n", location->file, location->line, location->column);
	} else {
		printf("[ubsan]	caught undefined behavior in %s:%u:%u\n", "unknown", 0U, 0U);
	}
}

noreturn void __ubsan_handle_type_mismatch(ubsan_type_mismatch_t *type_mismatch, uintptr_t pointer) {
	ubsan_location(&type_mismatch->location);

	if(pointer == 0) {
		die("[ubsan]	null pointer access\n");
	} else if(type_mismatch->alignment != 0 && is_aligned(pointer, type_mismatch->alignment)) {
		die("[ubsan]	unaligned memory access\n");
	} else {
		die("[ubsan]	%s address %p with insufficient space for object of type %s\n",
			type_check_kinds[type_mismatch->type_check_kind], (void *) pointer, type_mismatch->type->name);
	}

	__builtin_unreachable();
}

noreturn void __ubsan_handle_out_of_bounds(ubsan_out_of_bounds_t *out_of_bounds, uintptr_t index) {
	ubsan_location(&out_of_bounds->location);

	die("[ubsan]	index %u is out of bounds\n", index);

	__builtin_unreachable();
}

noreturn void __ubsan_handle_negate_overflow(ubsan_location_t *location, uintptr_t value) {
	ubsan_location(location);

	die("[ubsan]	%u; negation overflow\n", value);

	__builtin_unreachable();
}

noreturn void __ubsan_handle_sub_overflow(ubsan_location_t *location, uintptr_t left, uintptr_t right) {
	ubsan_location(location);

	die("[ubsan]	%u - %u; subtraction overflow\n", left, right);

	__builtin_unreachable();
}

noreturn void __ubsan_handle_add_overflow(ubsan_location_t *location, uintptr_t left, uintptr_t right) {
	ubsan_location(location);

	die("[ubsan]	%u + %u; addition overflow\n", left, right);

	__builtin_unreachable();
}

noreturn void __ubsan_handle_mul_overflow(ubsan_location_t *location, uintptr_t left, uintptr_t right) {
	ubsan_location(location);

	die("[ubsan]	%u * %u; multiplication overflow\n", left, right);

	__builtin_unreachable();
}

noreturn void __ubsan_handle_divrem_overflow(ubsan_location_t *location, uintptr_t left, uintptr_t right) {
	ubsan_location(location);

	die("[ubsan]	%u divrem %u; divrem overflow\n", left, right);

	__builtin_unreachable();
}

noreturn void __ubsan_handle_shift_out_of_bounds(ubsan_location_t *location, uintptr_t left, uintptr_t right) {
	ubsan_location(location);

	die("[ubsan]	shift %u by %u; shift out of bounds\n", left, right);

	__builtin_unreachable();
}

noreturn void __ubsan_handle_nonnull_arg(ubsan_nonnull_arg_t *data) {
	ubsan_location(&data->loc);

	die("[ubsan]	argument %d null, but required to be non-null by %s:%u:%u\n", data->argument, data->attr_loc.file, data->attr_loc.line,
		data->attr_loc.column);
}

noreturn void __ubsan_handle_builtin_unreachable(ubsan_location_t *location) {
	ubsan_location(location);

	die("[ubsan]	called __builtin_unreachable()\n");
}
