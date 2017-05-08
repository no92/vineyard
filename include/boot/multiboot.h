#pragma once

#include <stdint.h>

#define MB2_MAGIC 0x36D76289

typedef struct {
	uint32_t total_size;
	uint32_t reserved;
} __attribute__((packed)) multiboot2_t;

typedef struct {
	uint32_t type;
	uint32_t size;

	union {
		struct {
			uint32_t framebuffer_addr;
			uint32_t reserved;
			uint32_t framebuffer_pitch;
			uint32_t framebuffer_width;
			uint32_t framebuffer_height;
			uint8_t framebuffer_bpp;
			uint8_t framebuffer_type;
		} __attribute__((packed)) video;
	};
} __attribute__((packed)) multiboot2_tag_t;

multiboot2_tag_t *multiboot2_get_tag(multiboot2_t *multiboot, uint32_t type);
