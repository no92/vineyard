#include <init/panic.h>
#include <proc/elf32.h>
#include <util/trace.h>

#include <_/vineyard.h>
#include <stdio.h>
#include <string.h>

elf32_sym_t *symtab;
size_t strtab_len;

uintptr_t strtab;
size_t symtab_len;

void trace_init(multiboot2_t *multiboot) {
	multiboot2_tag_t *tag = multiboot2_get_tag(multiboot, MB2_TYPE_SYMBOLS);

	if(!tag) {
		panic("no symbol tag found");
	}

	elf32_shdr_t *shdr = (elf32_shdr_t *) tag->symbols.addr;
	uintptr_t shstrtab = shdr[tag->symbols.shndx].sh_addr + 0xC0000000;

	for(size_t i = 0; i < tag->symbols.num; i++, shdr++) {
		if(!shdr->sh_addr) {
			continue;
		}

		const char *name = (const char *) (shstrtab + shdr->sh_name);

		if(!strcmp(name, ".symtab")) {
			symtab = (elf32_sym_t *) (shdr->sh_addr + 0xC0000000);
			symtab_len = shdr->sh_size;
		} else if(!strcmp(name, ".strtab")) {
			strtab = shdr->sh_addr + 0xC0000000;
			strtab_len = shdr->sh_size;
		}
	}

	if(!symtab) {
		panic("no symtab found");
	} else if(!strtab) {
		panic("no strtab found");
	}
}


const char *trace_lookup_addr(uintptr_t addr) {
	elf32_sym_t *symbol = symtab;

	for(size_t i = 0; i < (symtab_len / sizeof(*symtab)); i++, symbol++) {
		if(ELF32_ST_TYPE(symbol->st_info) != ELF32_ST_TYPE_FUNCTION) {
			continue;
		}

		uintptr_t start = (uintptr_t) symbol->st_value;

		if(addr >= start && addr < (start + symbol->st_size)) {
			return (const char *) (strtab + symbol->st_name);
		}
	}

	return "???";
}


uintptr_t trace_lookup_name(const char *name) {
	elf32_sym_t *symbol = symtab;

	for(size_t i = 0; i < (symtab_len / sizeof(*symtab)); i++, symbol++) {
		if(ELF32_ST_TYPE(symbol->st_info) != ELF32_ST_TYPE_FUNCTION) {
			continue;
		}

		if(!strcmp(name, (const char *) (strtab + symbol->st_name))) {
			return symbol->st_value;
		}
	}

	return 0;
}

extern uintptr_t _text_start;
extern uintptr_t _text_end;

void trace(size_t levels) {
	uintptr_t *ebp;
	uintptr_t start = (uintptr_t) &_text_start;
	uintptr_t end = (uintptr_t) &_text_end;

	asm volatile ("mov %%ebp, %0" : "=r" (ebp));

	puts("[trace] stack trace");

	while(ebp && levels --> 0) {
		uintptr_t eip = ebp[1];

		if(eip == 0 || eip < start || eip > end) {
			return;
		}

		printf("[trace]	<%s> (%#08x)\n", trace_lookup_addr(eip), eip);

		ebp = (uintptr_t *) ebp[0];
	}
}
