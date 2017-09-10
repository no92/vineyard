#include <fs/vfs.h>

#include <stdlib.h>
#include <stdio.h>

uintptr_t sys_write(syscall_args_t *data) {
	return (uintptr_t) vfs_write((int) data->arg1, (void *) data->arg2, data->arg3);
}

ssize_t vfs_write(int fd, const void *buf, size_t count) {
	if(fd == 1 || fd == 2) {
		char *b = malloc(count + 1);
		snprintf(b, count + 1, "%s", (char *) buf);
		printf("%s", b);

		return (ssize_t) count;
	} else {
		panic("[vfs]	writing unimplemented");
	}
}
