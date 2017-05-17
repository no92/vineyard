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
