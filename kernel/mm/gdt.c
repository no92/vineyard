#include <mm/gdt.h>
#include <string.h>

#define GDT_ENTRIES 256

/* GDT access bits */
#define GDT_RW			1 << 1
#define GDT_X			1 << 3
#define GDT_USER		3 << 5
#define GDT_PRESENT		1 << 7

/* GDT flag bits */
#define GDT_GRANULARITY	1 << 3
#define GDT_PMODE		1 << 2

gdt_entry_t entries[GDT_ENTRIES];
gdtr_t gdtr;

static void gdt_set(size_t entry, uintptr_t base, uint32_t limit, uint8_t access, uint8_t flags) {
	entries[entry].base_low = base & 0xFFFF;
	entries[entry].base_mid = (base >> 16) & 0xFF;
	entries[entry].base_high = (uint8_t) (base >> 24) & 0xFF;

	entries[entry].limit_low = limit & 0xFFFF;
	entries[entry].limit_high = (limit >> 16) & 0xF;

	entries[entry].access = access | 0x10;
	entries[entry].flags = (uint8_t) (flags & 0xF);
}

void gdt_init(void) {
	memset(entries, 0, sizeof(entries));

	/* null segment */
	gdt_set(0, 0, 0, 0, 0);
	/* kernel code */
	gdt_set(1, 0, 0xFFFFFFFF, GDT_PRESENT | GDT_X | GDT_RW, GDT_GRANULARITY | GDT_PMODE);
	/* kernel data */
	gdt_set(2, 0, 0xFFFFFFFF, GDT_PRESENT | GDT_RW, GDT_GRANULARITY | GDT_PMODE);
	/* user code */
	gdt_set(3, 0, 0xFFFFFFFF, GDT_PRESENT | GDT_X | GDT_RW | GDT_USER, GDT_GRANULARITY | GDT_PMODE);
	/* user data */
	gdt_set(4, 0, 0xFFFFFFFF, GDT_PRESENT | GDT_RW | GDT_USER, GDT_GRANULARITY | GDT_PMODE);

	gdtr.limit = sizeof(entries) - 1;
	gdtr.addr = (uintptr_t) &entries;

	asm volatile ("lgdt %0" : : "m" (gdtr));
}
