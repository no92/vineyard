#pragma once

#include <stddef.h>

__attribute__((pure, nonnull(1))) size_t strlen(const char *s);

__attribute__((pure, nonnull(1, 2))) int strcmp(const char *s1, const char *s2);
__attribute__((pure, nonnull(1, 2))) int strncmp(const char *s1, const char *s2, size_t n);

__attribute__((nonnull(1, 2))) char *strcpy(char * restrict s1, const char * restrict s2);
__attribute__((nonnull(1, 2))) char *strncpy(char * restrict s1, const char * restrict s2, size_t n);

__attribute__((nonnull(1, 2))) char *strcat(char * restrict s1, const char * restrict s2);
__attribute__((nonnull(1, 2))) char *strncat(char * restrict s1, const char * restrict s2, size_t n);

__attribute__((nonnull(1, 2))) size_t strspn(const char *s1, const char *s2);

__attribute__((nonnull(2))) size_t strxfrm(char * restrict s1, const char * restrict s2, size_t n);

__attribute__((pure, nonnull(1))) void *memchr(const void *s, int c, size_t n);
__attribute__((pure, nonnull(1, 2))) int memcmp(const void *s1, const void *s2, size_t n);
__attribute__((nonnull(1, 2))) void *memcpy(void * restrict s1, const void * restrict s2, size_t n);
__attribute__((nonnull(1, 2))) void *memmove(void *s1, const void *s2, size_t n);
__attribute__((nonnull(1))) void *memset(void *s, int c, size_t n);

/* POSIX extensions */
__attribute__((pure, nonnull(1))) size_t strnlen(const char *s, size_t maxlen);
