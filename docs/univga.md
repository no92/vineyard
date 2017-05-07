# uni-vga font generation #

## prerequisites ##
The bdf file residies at `third-party/uni-vga/u_vga16.bdf`. The Adobe BDF format is a plain-text format. It encodes bitmap representations of characters line-by-line.

For our intents and purposes, a stripped-down version of this might look like this:
```
ENCODING 33
BITMAP
00
00
18
3C
3C
3C
18
18
18
00
18
18
00
00
00
00
```

As you might see by the code (0x21), the symmetry and the value of the nibbles, this represents an exclamation mark.

## Producing the font data ##
This is pretty simple: we just sequentially add every bitmap byte to the array. In our example, it would look like this:

```c
#include <stdint.h>

const uint8_t gfx_font[] = {
	// 0
	0x00, 0x00, 0xDA, 0x02,
	0x80, 0x82, 0x02, 0x80,
	0x82, 0x02, 0x80, 0xB6,
	0x00, 0x00, 0x00, 0x00,
	// 33
	0x00, 0x00, 0x18, 0x3C,
	0x3C, 0x3C, 0x18, 0x18,
	0x18, 0x00, 0x18, 0x18,
	0x00, 0x00, 0x00, 0x00,
};
```

## Producing the lookup table ##

Because we don't include characters that are not defined in the font, we will set them to zero, which is a placeholder character. For all others, we set the value to the offset of the first line, shifted to the right by 4:

```c
const uint16_t gfx_font_index[] = {
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	1
};
```
