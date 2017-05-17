# make sure we're running bash
SHELL		:= /bin/bash

ISO			:= bin/vineyard.iso

# define 'all' as the default target
all: third-party $(ISO)

include third-party/Makefile

# define various tools
CC			:= $(CROSS_GCC)
LD			:= $(CROSS_GCC)
AR			:= $(CROSS_AR)
AS			:= $(YASM)
EMU			:= $(QEMU)

# commands
MKDIR		:= mkdir -p
CP			:= cp
WGET		:= wget -q

# various utilities
INFO		:= util/info
ERROR		:= util/error
KEYBOARD	:= $(shell util/keyboard)
VBOX		:= util/virtualbox
MUTE		:= 2>&1 /dev/null

# set some flags for running the tools
CFLAGS		:= -ffreestanding -finline-functions -fno-omit-frame-pointer -fsanitize=undefined -fstack-protector-all
CFLAGS		+= -ggdb3 -Ilibk/include -Iinclude -MD -nostdinc -pipe -std=gnu11 -O3
CFLAGS		+= -Wall -Wbad-function-cast -Wcast-align -Wconversion -Werror -Wextra -Wformat=2 -Wformat-signedness -Wimplicit-fallthrough=5 -Winit-self
CFLAGS		+= -Winline -Wlogical-op -Wmissing-braces -Wmissing-declarations -Wmissing-field-initializers -Wmissing-prototypes -Wnested-externs -Wparentheses
CFLAGS		+= -Wpedantic -Wpointer-arith -Wredundant-decls -Wshadow -Wstrict-prototypes -Wsuggest-attribute=format -Wsuggest-attribute=noreturn
CFLAGS		+= -Wsuggest-attribute=pure -Wswitch-default -Wswitch-enum -Wuninitialized -Wunreachable-code -Wunused -Wwrite-strings
LDFLAGS		:= -T build/kernel.ld -ffreestanding -nostdlib -lgcc
ASFLAGS		:= -f elf32 -g dwarf2
EMUARGS		:= -M accel=kvm:tcg -m 1024 -net none -serial stdio -rtc base=utc -vga std -k $(KEYBOARD)

include libk/Makefile
include kernel/Makefile

todo:
	@for file in $(shell find kernel libk -name '*.[chs]' -type f); do fgrep -H -e TODO -e FIXME $$file | sed -e 's/:\//: \//g' | sed -e 's/[[:space:]]/ /g'; done; true

clean: clean-kernel

distclean: clean-kernel clean-font clean-libk clean-libc clean-libc-test

.PHONY: all clean clean-deep test test-debug test-virtualbox
.SECONDARY:
.SUFFIXES:
