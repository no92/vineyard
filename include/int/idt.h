#pragma once

#include <stdint.h>

typedef struct {
	uint16_t offset_low;
	uint16_t selector;
	uint8_t reserved;
	uint8_t flags;
	uint16_t offset_high;
} __attribute__((packed)) idt_entry_t;

typedef struct {
	uint16_t limit;
	uintptr_t addr;
} __attribute__((packed)) idtr_t;

typedef struct {
	uint32_t edi;
	uint32_t esi;
	uint32_t ebp;
	uint32_t reserved;
	uint32_t ebx;
	uint32_t edx;
	uint32_t ecx;
	uint32_t eax;
	uint32_t interrupt;
	uint32_t error;
	uint32_t eip;
	uint32_t cs;
	uint32_t eflags;
	uint32_t esp;
	uint32_t ss;
} __attribute__((packed)) frame_t;

void idt_init(void);
void handler(frame_t *state);

extern void exception0(void);
extern void exception1(void);
extern void exception2(void);
extern void exception3(void);
extern void exception4(void);
extern void exception5(void);
extern void exception6(void);
extern void exception7(void);
extern void exception8(void);
extern void exception9(void);
extern void exception10(void);
extern void exception11(void);
extern void exception12(void);
extern void exception13(void);
extern void exception14(void);
extern void exception15(void);
extern void exception16(void);
extern void exception17(void);
extern void exception18(void);
extern void exception19(void);
extern void exception20(void);
extern void exception21(void);
extern void exception22(void);
extern void exception23(void);
extern void exception24(void);
extern void exception25(void);
extern void exception26(void);
extern void exception27(void);
extern void exception28(void);
extern void exception29(void);
extern void exception30(void);
extern void exception31(void);
