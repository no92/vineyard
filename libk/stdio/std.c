#include <stdio.h>

static FILE _stdin = {
	.fd = 0,
};

static FILE _stdout = {
	.fd = 1,
};

static FILE _stderr = {
	.fd = 2,
};

FILE * const stdin = &_stdin;
FILE * const stdout = &_stdout;
FILE * const stderr = &_stderr;
