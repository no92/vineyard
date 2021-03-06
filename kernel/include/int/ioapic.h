#pragma once

#include <util/list.h>
#include <stdint.h>

#define REDTBL_TRIGGER_LEVEL    0x0000000000008000
#define REDTBL_ACTIVE_HIGH      0x0000000000002000

#define REDTBL_DESTMODE_LOGICAL  0x0000000000000800
#define REDTBL_DESTMODE_PHYSICAL 0x0000000000000000
#define REDTBL_DELMODE_FIXED     0x0000000000000000
#define REDTBL_DELMODE_LOWPRI    0x0000000000000100

typedef struct {
	size_t id;
	node_t node;
	volatile uint32_t *reg;
	volatile uint32_t *val;
	size_t irq_base;
	size_t irqs;
} ioapic_t;

void ioapic_init(size_t id, uintptr_t addr, size_t irq_base);
void ioapic_route(size_t from, uintptr_t flags, size_t to);
