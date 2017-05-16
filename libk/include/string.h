#pragma once

#include <stddef.h>

__attribute__((pure, nonnull(1))) size_t strlen(const char *s);

__attribute__((pure, nonnull(1, 2))) int strcmp(const char *s1, const char *s2);
__attribute__((pure, nonnull(1, 2))) int strncmp(const char *s1, const char *s2, size_t n);

char *strcpy(char * restrict s1, const char * restrict s2);
char *strncpy(char * restrict s1, const char * restrict s2, size_t n);

char *strcat(char * restrict s1, const char * restrict s2);
char *strncat(char * restrict s1, const char * restrict s2, size_t n);

size_t strspn(const char *s1, const char *s2);


__attribute__((pure, nonnull(1))) void *memchr(const void *s, int c, size_t n);
__attribute__((pure, nonnull(1, 2))) int memcmp(const void *s1, const void *s2, size_t n);
void *memcpy(void * restrict s1, const void * restrict s2, size_t n);
void *memmove(void *s1, const void *s2, size_t n);
void *memset(void *s, int c, size_t n);
