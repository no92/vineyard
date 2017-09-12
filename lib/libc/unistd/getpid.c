#include <_/syscall.h>
#include <unistd.h>

pid_t getpid(void) {
#ifdef __libk
	return 0;
#else
	return SYSCALL0(0x02);
#endif
}

#ifdef UNIT
#include <_libk_test.h>

int main(void) {
	return EXIT_SUCCESS;
}

#endif
