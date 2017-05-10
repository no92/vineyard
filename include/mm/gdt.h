#pragma once

#include <stdint.h>

typedef struct {
	uint16_t limit;
	uintptr_t addr;
} __attribute__((packed)) gdtr_t;

typedef struct {
	uint16_t limit_low;
	uint16_t base_low;
	uint8_t base_mid;
	uint8_t access;
	uint8_t limit_high: 4;
	uint8_t flags: 4;
	uint8_t base_high;
} __attribute__((packed)) gdt_entry_t;

void gdt_init(void);
