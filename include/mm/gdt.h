#pragma once

#include <stddef.h>
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

#define GDT_ENTRIES 6

#define GDT_KERNEL_CODE	0x08
#define GDT_KERNEL_DATA	0x10
#define GDT_USER_CODE	0x18
#define GDT_USER_DATA	0x20
#define GDT_TSS			0x28

extern gdt_entry_t gdt[GDT_ENTRIES];
extern gdtr_t gdtr;

void gdt_init(void);
void gdt_set(size_t entry, uintptr_t base, uint32_t limit, uint8_t access, uint8_t flags);
