#include <cpu/ports.h>
#include <int/handler.h>
#include <time/pit.h>
#include <proc/scheduler.h>

/* milliseconds since boot */
static volatile uint32_t counter = 0;

uint32_t pit_uptime(void) {
	return counter;
}

static void pit_tick(frame_t *state) {
	counter++;

	scheduler_tick(state);
}

void pit_init(void) {
	handler_set(32, &pit_tick);

	uint32_t divisor = 1193180 / 1000;

	outb(0x43, 0x36);
	outb(0x40, divisor & 0xFF);
	outb(0x40, (divisor >> 8) & 0xFF);
}
