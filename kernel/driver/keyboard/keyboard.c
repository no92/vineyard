#include <cpu/ports.h>
#include <driver/keyboard.h>
#include <int/handler.h>

#include <_/vineyard.h>
#include <ctype.h>
#include <stdio.h>

uint32_t code = 0;

static const struct keymap *keyboard_conversion(uint32_t c) {
	for(size_t i = 0; i < sizeof(keyboard_conversion_table) / sizeof(keyboard_conversion_table[0]); i++) {
		if(c == keyboard_conversion_table[i].scan_press || c == keyboard_conversion_table[i].scan_release) {
			return &keyboard_conversion_table[i];
		}
	}

	return NULL;
}

A("bitwise operator in conditional")
static void keyboard_press(frame_t *state) {
	(void) state;

	struct event_input keypress;
	const struct keymap *key;
	uint8_t c = inb(0x60);

	if((c & 0xFE) == 0xE0) {
		code |= c;
		return;
	}

	if(code == 0) {
		code = c;
	} else {
		code <<= 8;
		code |= c;
	}

	key = keyboard_conversion(code);

	if(key != NULL) {
		keypress.code = key->keycode;

		if(c == code) {
			keypress.type = (c & 0x80) ? KEY_RELEASE : KEY_PRESS;
		} else {
			keypress.type = ((code >> 8) & 0x80) ? KEY_RELEASE : KEY_PRESS;
		}

		code = 0;

		if(keypress.type == KEY_PRESS && (islower(*key->name) || iscntrl(*key->name))) {
			printf("%s", key->name);
		}
	}
}

void keyboard_init(void) {
	handler_set(0x21, &keyboard_press);
}
