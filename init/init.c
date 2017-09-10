#include <stdio.h>
#include <unistd.h>

int main(void) {
	printf("[init]	pid = %u\n", getpid());
	return 0;
}
