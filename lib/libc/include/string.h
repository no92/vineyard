#pragma once

#include <stddef.h>

__attribute__((nonnull(1, 2))) void *memccpy(void * restrict dest, const void * restrict src, int c, size_t n);
__attribute__((nonnull(1, 2))) void *memcpy(void * restrict s1, const void * restrict s2, size_t n);
__attribute__((nonnull(1, 2))) void *memmove(void *s1, const void *s2, size_t n);

__attribute__((nonnull(1, 2))) char *strcpy(char * restrict s1, const char * restrict s2);
__attribute__((nonnull(1, 2))) char *strncpy(char * restrict s1, const char * restrict s2, size_t n);

__attribute__((nonnull(1, 2))) char *strcat(char * restrict s1, const char * restrict s2);
__attribute__((nonnull(1, 2))) char *strncat(char * restrict s1, const char * restrict s2, size_t n);

__attribute__((pure, nonnull(1, 2))) int memcmp(const void *s1, const void *s2, size_t n);
__attribute__((pure, nonnull(1, 2))) int strcmp(const char *s1, const char *s2);
/* int strcoll(const char *s1, const char *s2); */
__attribute__((pure, nonnull(1, 2))) int strncmp(const char *s1, const char *s2, size_t n);

__attribute__((nonnull(2))) size_t strxfrm(char * restrict s1, const char * restrict s2, size_t n);

__attribute__((pure, nonnull(1))) void *memchr(const void *s, int c, size_t n);
__attribute__((pure, nonnull(1))) char *strchr(const char *s, int c);

/* size_t strcspn(const char *1, const char *s2); */
/* char *strpbrk(const char *s1, const char *s2); */
/* char *strrchr(const char *s, int c); */

__attribute__((pure, nonnull(1, 2))) size_t strspn(const char *s1, const char *s2);

__attribute__((nonnull(1, 2))) char *strstr(const char *s1, const char *s2);
/* char *strtok(char * restrict s1, const char *restrict s2) */

__attribute__((nonnull(1))) void *memset(void *s, int c, size_t n);

/* char *strerror(int errnum) */

__attribute__((pure, nonnull(1))) size_t strlen(const char *s);

/* POSIX extensions */
/* void *memccpy(void *s1, const void *s2, int c, size_t n); */
__attribute__((malloc)) char *strdup(const char *s1);
/* char *strtok_r(char *s, const char *sep, char **lasts); */

/* GNU extensions */
__attribute__((pure, nonnull(1))) size_t strnlen(const char *s, size_t maxlen);
