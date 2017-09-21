#include <string.h>
#include <stdio.h>

#include <mm/gdt.h>
#include <mm/virtual.h>
#include <proc/tss.h>

tss_t tss;
extern uintptr_t stack_top;

void tss_init(void) {
	memset(&tss, 0x00, sizeof(tss));

	gdt_set(GDT_TSS >> 3, (uintptr_t) &tss, sizeof(tss_t) - 1, 0xE9, 0x00);

	tss.ss0 = GDT_KERNEL_DATA;
	tss.esp0 = (uintptr_t) &stack_top;
	tss.ss = 0x13;
	tss.es = 0x13;
	tss.cs = 0x0B;
	tss.ds = 0x13;
	tss.fs = 0x13;
	tss.gs = 0x13;
	tss.iomap_base = sizeof(tss);
}

void tss_set_esp(uintptr_t esp) {
	tss.esp0 = esp;
}
