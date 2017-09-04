#pragma once

#include <init/multiboot2.h>

typedef struct {
    char filename[100];
    char mode[8];
    char uid[8];
    char gid[8];
    char size[12];
    char mtime[12];
    char chksum[8];
    char typeflag[1];
} initrd_file_t;

void initrd_init(multiboot2_t *multiboot);
uint32_t initrd_file_size(const char *in);
initrd_file_t *initrd_open(const char *file);
