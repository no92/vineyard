#include <_/vineyard.h>
#include <ctype.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#define MIN(a, b) (((a) < (b)) ? (a) : (b))

#define FLAG_HASH	1 << 0
#define FLAG_PLUS	1 << 1
#define FLAG_SPACE	1 << 2
#define FLAG_ZERO	1 << 3
#define FLAG_MINUS	1 << 4

#define LENGTH_hh	1 << 0
#define LENGTH_h	1 << 1
#define LENGTH_l	1 << 2
#define LENGTH_ll	1 << 3
#define LENGTH_L	1 << 4
#define LENGTH_z	1 << 5
#define LENGTH_j	1 << 6
#define LENGTH_t	1 << 7

static char *utoa(uint64_t value, char *str, uint8_t base) {
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

static char *itoa(int64_t value, char *str, uint8_t base) {
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

A("bitwise operator in conditional")
int vsnprintf(char * restrict s, size_t n, const char * restrict format, va_list arg) {
	int flags = 0;
	size_t width = 0;
	size_t length_orig = n;
	size_t length = 0;

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

			/* handle length */
			if(!strncmp(format, "ll", 2)) {
				length |= LENGTH_ll;
				format += 2;
			} else if(!strncmp(format, "hh", 2)) {
				length |= LENGTH_hh;
				format += 2;
			} else if(*format == 'l') {
				length |= LENGTH_l;
				format++;
			} else if(*format == 'h') {
				length |= LENGTH_h;
				format++;
			} else if(*format == 'z') {
				length |= LENGTH_z;
				format++;
			} else if(*format == 'j') {
				length |= LENGTH_j;
				format++;
			} else if(*format == 't') {
				length |= LENGTH_t;
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
					length = 0;
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
					uint64_t val = 0;

					if(length == 0 || length == LENGTH_l) {
						val = va_arg(arg, unsigned int);
					} else if(length == LENGTH_hh) {
						val = va_arg(arg, unsigned int);
						val &= 0xFF;
					} else if(length == LENGTH_h) {
						val = va_arg(arg, unsigned int);
						val &= 0xFFFF;
					} else if(length == LENGTH_ll || length == LENGTH_j) {
						val = va_arg(arg, uint64_t);
					} else if(length == LENGTH_z) {
						val = va_arg(arg, size_t);
					} else if(length == LENGTH_t) {
						val = (uint64_t) va_arg(arg, ptrdiff_t);
					}

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
					uint64_t val = 0;

					if(length == 0 || length == LENGTH_l) {
						val = va_arg(arg, unsigned int);
					} else if(length == LENGTH_hh) {
						val = va_arg(arg, unsigned int);
						val &= 0xFF;
					} else if(length == LENGTH_h) {
						val = va_arg(arg, unsigned int);
						val &= 0xFFFF;
					} else if(length == LENGTH_ll || length == LENGTH_j) {
						val = va_arg(arg, uint64_t);
					} else if(length == LENGTH_z) {
						val = va_arg(arg, size_t);
					} else if(length == LENGTH_t) {
						val = (uint64_t) va_arg(arg, ptrdiff_t);
					}

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
					char buf[64];
					int64_t val = 0;

					if(length == 0 || length == LENGTH_l) {
						val = va_arg(arg, signed int);
					} else if(length == LENGTH_hh) {
						val = va_arg(arg, signed int);
						val &= 0xFF;
					} else if(length == LENGTH_h) {
						val = va_arg(arg, signed int);
						val &= 0xFFFF;
					} else if(length == LENGTH_ll || length == LENGTH_j) {
						val = va_arg(arg, int64_t);
					} else if(length == LENGTH_z) {
						val = va_arg(arg, ssize_t);
					} else if(length == LENGTH_t) {
						val = (int64_t) va_arg(arg, ptrdiff_t);
					}

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
					char buf[64];
					uint64_t val = 0;

					if(length == 0 || length == LENGTH_l) {
						val = va_arg(arg, unsigned int);
					} else if(length == LENGTH_hh) {
						val = va_arg(arg, unsigned int);
						val &= 0xFF;
					} else if(length == LENGTH_h) {
						val = va_arg(arg, unsigned int);
						val &= 0xFFFF;
					} else if(length == LENGTH_ll || length == LENGTH_j) {
						val = va_arg(arg, uint64_t);
					} else if(length == LENGTH_z) {
						val = va_arg(arg, size_t);
					} else if(length == LENGTH_t) {
						val = (uint64_t) va_arg(arg, ptrdiff_t);
					}

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
			length = 0;
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
