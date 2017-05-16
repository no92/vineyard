#include <init/panic.h>

#include <stddef.h>
#include <stdint.h>
#include <stdnoreturn.h>

/* TODO: move this to libk? */

#define RDRAND_RETRIES 20

uintptr_t __stack_chk_guard;

__attribute__((constructor)) void __stack_chk_setup(void);
noreturn void __stack_chk_fail(void);

__attribute__((constructor)) void __stack_chk_setup(void) {
	uint32_t canary;
	int done = 0;
	size_t counter = 0;

	while(!done && counter < RDRAND_RETRIES) {
		asm (
			"rdseed %%eax;"
			"mov $1, %%edx;"
			"cmovae %%eax, %%edx;"
			"mov %%edx, %1;"
			"mov %%eax, %0"
		: "=r" (canary), "=r" (done) : : "%eax", "%edx");

		counter++;
	}

	if(canary == 0) {
		canary = 0xE2DEE396;
	}

	__stack_chk_guard = canary;
}

noreturn void __stack_chk_fail(void) {
	panic("stack smashed!");
}
