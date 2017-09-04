#pragma once

#include <stddef.h>
#include <stdint.h>

typedef struct {
	uint32_t edi;
	uint32_t esi;
	uint32_t ebp;
	uint32_t reserved;
	uint32_t ebx;
	uint32_t edx;
	uint32_t ecx;
	uint32_t eax;
	uint32_t gs;
	uint32_t fs;
	uint32_t es;
	uint32_t ds;
	uint32_t interrupt;
	uint32_t error;
	uint32_t eip;
	uint32_t cs;
	uint32_t eflags;
	uint32_t esp;
	uint32_t ss;
} __attribute__((packed)) frame_t;

typedef void (*isr_handler_t)(frame_t *);

void handler(frame_t *state);
void handler_set(size_t n, isr_handler_t addr);
