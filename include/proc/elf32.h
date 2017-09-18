#pragma once

#include <stddef.h>
#include <stdint.h>

#define ELF32_ST_TYPE(i)		((i) & 0x0F)

#define ELF32_ST_TYPE_NONE		0
#define ELF32_ST_TYPE_OBJECT	1
#define ELF32_ST_TYPE_FUNCTION	2
#define ELF32_ST_TYPE_SECTION	3
#define ELF32_ST_TYPE_FILE		4
#define ELF32_ST_TYPE_LOPROC	13
#define ELF32_ST_TYPE_HIPROC	15

typedef struct {
	uint8_t e_ident[16];
	uint16_t e_type;
	uint16_t e_machine;
	uint32_t e_version;
	uint32_t e_entry;
	uint32_t e_phoff;
	uint32_t e_shoff;
	uint32_t e_flags;
	uint16_t e_ehsize;
	uint16_t e_phentsize;
	uint16_t e_phnum;
	uint16_t e_shentsize;
	uint16_t e_shnum;
	uint16_t e_shstrndx;
} elf32_header_t;

typedef struct {
	uint32_t p_type;
	uint32_t p_offset;
	uint32_t p_vaddr;
	uint32_t p_paddr;
	uint32_t p_filesz;
	uint32_t p_memsz;
	uint32_t p_flags;
	uint32_t p_align;
} elf32_program_header_t;

typedef struct {
	uint32_t sh_name;
	uint32_t sh_type;
	uint32_t sh_flags;
	uint32_t sh_addr;
	uint32_t sh_offset;
	uint32_t sh_size;
	uint32_t sh_link;
	uint32_t sh_info;
	uint32_t sh_addralign;
	uint32_t sh_entsize;
} elf32_shdr_t;

typedef struct {
	uint32_t st_name;
	uint32_t st_value;
	uint32_t st_size;
	uint8_t st_info;
	uint8_t st_other;
	uint16_t st_shndx;
} elf32_sym_t;

uintptr_t elf32_load(const char *path);
