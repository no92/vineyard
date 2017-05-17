#include <cpu/ports.h>
#include <driver/keyboard.h>
#include <int/handler.h>

#include <stdio.h>

static void keyboard_press(frame_t *state) {
	char c = (char) inb(0x60);

	(void) state;
	(void) c;
}

void keyboard_init(void) {
	handler_set(0x21, &keyboard_press);
}
