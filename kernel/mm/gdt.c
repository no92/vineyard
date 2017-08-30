#include <mm/gdt.h>
#include <proc/tss.h>

#include <string.h>
#include <stdio.h>

/* GDT access bits */
#define GDT_RW			1 << 1
#define GDT_X			1 << 3
#define GDT_USER		3 << 5
#define GDT_PRESENT		1 << 7

/* GDT flag bits */
#define GDT_GRANULARITY	1 << 3
#define GDT_PMODE		1 << 2

gdt_entry_t gdt[GDT_ENTRIES];
gdtr_t gdtr;

void gdt_set(size_t entry, uintptr_t base, uint32_t limit, uint8_t access, uint8_t flags) {
	gdt[entry].base_low = base & 0xFFFF;
	gdt[entry].base_mid = (base >> 16) & 0xFF;
	gdt[entry].base_high = (uint8_t) (base >> 24) & 0xFF;

	gdt[entry].limit_low = limit & 0xFFFF;
	gdt[entry].limit_high = (limit >> 16) & 0xF;

	gdt[entry].access = access;
	gdt[entry].flags = (uint8_t) (flags & 0xF);
}

extern void gdt_reload(void);

void gdt_init(void) {
	memset(gdt, 0, sizeof(gdt));

	/* kernel code */
	gdt_set(GDT_KERNEL_CODE >> 3, 0, 0xFFFFFFFF, GDT_PRESENT | GDT_X | GDT_RW | 0x10, GDT_GRANULARITY | GDT_PMODE);
	/* kernel data */
	gdt_set(GDT_KERNEL_DATA >> 3, 0, 0xFFFFFFFF, GDT_PRESENT | GDT_RW | 0x10, GDT_GRANULARITY | GDT_PMODE);
	/* user code */
	gdt_set(GDT_USER_CODE >> 3, 0, 0xFFFFFFFF, GDT_PRESENT | GDT_X | GDT_RW | GDT_USER | 0x10, GDT_GRANULARITY | GDT_PMODE);
	/* user data */
	gdt_set(GDT_USER_DATA >> 3, 0, 0xFFFFFFFF, GDT_PRESENT | GDT_RW | GDT_USER | 0x10, GDT_GRANULARITY | GDT_PMODE);
	/* TSS */
	tss_init();

	gdtr.limit = sizeof(gdt) - 1;
	gdtr.addr = (uintptr_t) &gdt;

	gdt_reload();
	tss_ltr();
}
