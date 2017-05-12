#include <ctype.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MIN(a, b) (((a) < (b)) ? (a) : (b))

#define FLAG_HASH 1 << 0
#define FLAG_PLUS 1 << 1
#define FLAG_SPACE 1 << 2
#define FLAG_ZERO 1 << 3
#define FLAG_MINUS 1 << 4

static char *utoa(uint32_t value, char *str, uint8_t base) {
	if(base < 1 || base > 36) {
		return 0;
	}

	int pos = 0;

	do {
		str[pos] = (char) (value % base + '0');

		if(str[pos] > '9') {
			str[pos] = (char) (str[pos] + 7);
		}

		pos++;
	} while((value /= base) > 0);

	for(int i = 0, j = pos - 1; i < j; i++, j--) {
		char tmp = str[i];
		str[i] = str[j];
		str[j] = tmp;
	}

	str[pos] = '\0';

	return str;
}

static char *itoa(int32_t value, char *str, uint8_t base) {
	if(base < 1 || base > 36) {
		return 0;
	}

	uint32_t val = (uint32_t) ((value < 0) ? -value : value);
	int pos = 0;

	do {
		str[pos] = (char) (val % base + '0');

		if(str[pos] > '9') {
			str[pos] = (char) (str[pos] + 7);
		}

		pos++;
	} while((val /= base) > 0);

	if(value < 0) {
		str[pos++] = '-';
	}

	for(int i = 0, j = pos - 1; i < j; i++, j--) {
		char tmp = str[i];
		str[i] = str[j];
		str[j] = tmp;
	}

	str[pos] = '\0';

	return str;
}

int vsnprintf(char * restrict s, size_t n, const char * restrict format, va_list arg) {
	int flags = 0;
	size_t width = 0;
	size_t length_orig = n;

	while(*format && n > 1) {
		if(format[0] != '%') {
			*s++ = *format++;
			n--;
		} else if(format[0] == '%' && format[1] != '%') {
			/* skip the '%' symbol */
			format++;

flags:
			if(*format == '#') {
				flags |= FLAG_HASH;
				format++;
				goto flags;
			}

			if(*format == '+') {
				flags |= FLAG_PLUS;
				format++;
				goto flags;
			}

			if(*format == ' ') {
				flags |= FLAG_SPACE;
				format++;
				goto flags;
			}

			if(*format == '0') {
				flags |= FLAG_ZERO;
				format++;
				goto flags;
			}

			if(*format == '-') {
				flags |= FLAG_MINUS;
				format++;
				goto flags;
			}

width:
			if(isdigit(format[0])) {
				width *= 10;
				width += (size_t) (format[0] - '0');
				format++;
				goto width;
			} else if(format[0] == '*') {
				int w = va_arg(arg, int);

				if(w < 0) {
					width = (size_t) -w;
					flags |= FLAG_MINUS;
				} else {
					width = (size_t) w;
				}

				format++;
			}

			switch(format[0]) {
				case 'c': {
					char c = (char) va_arg(arg, int);
					*s++ = c;
					n--;
					break;
				}
				case 's': {
					const char *str = va_arg(arg, const char *);
					size_t len = strlen(str);

					while(len --> 0) {
						*s++ = *str++;
						n--;

						if(n <= 1) {
							goto exit;
						}
					}

					break;
				}
				case 'p': {
					flags |= FLAG_HASH;
					FALLTHROUGH
				}
				case 'X':
				case 'x': {
					if(flags & FLAG_HASH) {
						*s++ = '0';
						n--;

						if(n > 1) {
							*s++ = (format[0] == 'x') ? 'x' : 'X';
							n--;
						}
					}

					char buf[9];
					uint32_t val = va_arg(arg, uint32_t);
					utoa(val, buf, 0x10);
					size_t len = strlen(buf);

					if(len < width) {
						size_t diff = (width - len);
						char pad = (flags & FLAG_ZERO) ? '0' : ' ';

						diff = MIN(diff, n - 1);
						n -= diff;

						while(diff --> 0) {
							*s++ = pad;
						}
					}

					len = MIN(len, n - 1);

					if(format[0] == 'x') {
						for(size_t i = 0; i < len; i++) {
							*s++ = (char) tolower(buf[i]);
						}
					} else {
						strncpy(s, buf, len);
						s += len;
					}

					n -= len;

					break;
				}
				case 'u': {
					char buf[11];
					uint32_t val = va_arg(arg, uint32_t);
					utoa(val, buf, 10);
					size_t len = strlen(buf);

					if(len < width) {
						size_t diff = width - len;
						char pad = (flags & FLAG_ZERO) ? '0' : ' ';

						diff = MIN(diff, n - 1);
						n -= diff;

						while(diff --> 0) {
							*s++ = pad;
						}
					}

					len = MIN(len, n - 1);

					strncpy(s, buf, len);
					s += len;
					n -= len;

					break;
				}
				case 'd':
				case 'i': {
					char buf[12];
					int32_t val = va_arg(arg, int32_t);
					itoa(val, buf, 10);

					if(flags & FLAG_PLUS && val >= 0 && n > 1) {
						*s++ = '+';
						n--;
					} else if(flags & FLAG_SPACE && val >= 0 && n > 1) {
						*s++ = ' ';
						n--;
					}

					size_t len = strlen(buf);

					if(len < width) {
						size_t diff = width - len;
						char pad = (flags & FLAG_ZERO) ? '0' : ' ';

						diff = MIN(diff, n - 1);
						n -= diff;

						while(diff --> 0) {
							*s++ = pad;
						}
					}

					len = MIN(len, n - 1);

					strncpy(s, buf, len);
					s += len;
					n -= len;

					break;
				}
				case 'o': {
					char buf[12];
					uint32_t val = va_arg(arg, uint32_t);

					if(flags & FLAG_HASH) {
						*s++ = '0';
						n--;

						if(val == 0) {
							break;
						}
					}

					utoa(val, buf, 8);
					size_t len = strlen(buf);

					if(len < width) {
						size_t diff = width - len;
						char pad = (flags & FLAG_ZERO) ? '0' : ' ';

						diff = MIN(diff, n - 1);
						n -= diff;

						while(diff --> 0) {
							*s++ = pad;
						}
					}

					len = MIN(len, n - 1);

					strncpy(s, buf, len);
					s += len;
					n -= len;

					break;
				}
				default: {
					*s++ = '%';
					n--;

					if(n > 1) {
						*s++ = format[0];
						n--;
					}

					break;
				}
			}

			flags = 0;
			width = 0;
			format++;
		} else {
			/* handle percent symbol */
			*s++ = '%';
			n--;
		}
	}

exit:
	*s++ = '\0';
	return (int) (length_orig - n);
}

#ifdef UNIT
#include <_libk_test.h>

int main(void) {
	return EXIT_SUCCESS;
}

#endif
