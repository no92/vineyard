#pragma once

#include <proc/syscall.h>
#include <sys/types.h>

uintptr_t sys_write(syscall_args_t *data);
ssize_t vfs_write(int fd, const void *buf, size_t count);
