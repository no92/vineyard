#pragma once

#include <init/multiboot2.h>

#include <stddef.h>
#include <stdint.h>

#define LFB(x, y) (((x) * gfx_bpp) + ((y) * gfx_pitch))

uint32_t gfx_font_lookup[2];
const uint8_t gfx_font[46384];
const uint16_t gfx_font_index[11120];

uint32_t *gfx_framebuffer;
uint32_t gfx_framebuffer_size;
uint32_t gfx_width;
uint32_t gfx_height;
uint32_t gfx_pitch;
uint32_t gfx_bpp;

void gfx_init(multiboot2_t *multiboot);
void gfx_set(size_t h, size_t v, uint32_t color);
size_t gfx_draw_char(const char *c, size_t x, size_t y);

size_t gfx_putc(const char *c);
