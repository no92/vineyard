#include <_/syscall.h>
#include <stdlib.h>
#include <stdnoreturn.h>

extern void _fini(void);

noreturn void exit(int status) {
	_fini();
	SYSCALL1(0x00, status);

	__builtin_unreachable();
}
