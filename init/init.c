#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>

int main(void) {
	printf("[init]	pid = %u\n", getpid());

	return 0;
}
