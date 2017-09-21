#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>

int main(int argc, char *argv[]) {
	(void) argc;

	printf("[%s]	pid = %u\n", argv[0], getpid());

	return 0;
}
