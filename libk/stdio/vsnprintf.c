#include <ctype.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#define MIN(a, b) (((a) < (b)) ? (a) : (b))

#define FLAG_HASH 1 << 0

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

int vsnprintf(char * restrict s, size_t n, const char * restrict format, va_list arg) {
	int flags = 0;
	size_t length_orig = n;

	while(*format && n > 1) {
		if(format[0] != '%') {
			*s++ = *format++;
			n--;
		} else if(format[0] == '%' && format[1] != '%') {
			/* skip the '%' symbol */
			format++;

			if(format[0] == '#') {
				flags |= FLAG_HASH;
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
				case 'X':
				case 'x': {
					if(flags & FLAG_HASH) {
						*s++ = '0';
						n--;

						if(n > 1) {
							*s++ = 'x';
							n--;
						}
					}

					char buf[9];
					uint32_t val = va_arg(arg, uint32_t);
					utoa(val, buf, 0x10);
					size_t len = strlen(buf);
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
