#pragma once

#include <stdint.h>

#define MB2_MAGIC				0x36D76289

#define MB2_TYPE_MEMINFO		4
#define MB2_TYPE_MMAP			6
#define MB2_TYPE_FRAMEBUFFER	8
#define MB2_TYPE_SYMBOLS		9

typedef struct {
	uint32_t total_size;
	uint32_t reserved;
} __attribute__((packed)) multiboot2_t;

typedef struct {
	uint32_t type;
	uint32_t size;

	union {
		struct {
			uint32_t lower;
			uint32_t upper;
		} __attribute__((packed)) mem;

		struct {
			uint32_t entry_size;
			uint32_t entry_version;
		} __attribute__((packed)) map;

		struct {
			uint32_t framebuffer_addr;
			uint32_t zero;
			uint32_t framebuffer_pitch;
			uint32_t framebuffer_width;
			uint32_t framebuffer_height;
			uint8_t framebuffer_bpp;
			uint8_t framebuffer_type;
		} __attribute__((packed)) video;

		struct {
			uint32_t num;
			uint32_t entsize;
			uint32_t shndx;
			char addr[1];
		} __attribute__((packed)) symbols;
	};
} __attribute__((packed)) multiboot2_tag_t;

typedef struct {
	uint64_t addr;
	uint64_t length;
	uint32_t type;
	uint32_t reserved;
} __attribute__((packed)) multiboot2_map_entry_t;

multiboot2_tag_t *multiboot2_get_tag(multiboot2_t *multiboot, uint32_t type);
