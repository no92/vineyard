MAKEFLAGS	+= "-j$(shell grep -c ^processor /proc/cpuinfo 2> /dev/null || echo 1) "

# make sure we're running bash
SHELL		:= /bin/bash

ISO			:= bin/vineyard.iso

# define 'all' as the default target
all: third-party headers $(ISO)

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
VMWARE		:= util/vmware
WOL			:= util/wol
MUTE		:= 2>&1 /dev/null

# set some flags for running the tools
CFLAGS		?= -ggdb3 -pipe -O3
CFLAGS		+= -ffreestanding -finline-functions -fno-omit-frame-pointer -fsanitize=undefined -fstack-protector-all
CFLAGS		+= -Ilibk/include -Iinclude -MD -nostdinc -std=gnu11
CFLAGS		+= -Wall -Wbad-function-cast -Wconversion -Werror -Wextra -Wformat=2 -Winit-self -Wparentheses -Winline -Wmissing-braces
CFLAGS		+= -Wmissing-declarations -Wmissing-field-initializers -Wmissing-prototypes -Wnested-externs -Wpointer-arith
CFLAGS		+= -Wredundant-decls -Wshadow -Wstrict-prototypes -Wswitch-default -Wswitch-enum -Wuninitialized -Wunreachable-code
CFLAGS		+= -Wunused -Wwrite-strings
CFLAGS_GCC	:= -Wformat-signedness -Wlogical-op -Wrestrict -Wsuggest-attribute=format -Wsuggest-attribute=noreturn -Wsuggest-attribute=pure
LDFLAGS		:= -T build/kernel.ld -ffreestanding -nostdlib -lgcc
ASFLAGS		:= -f elf32 -g dwarf2
EMUARGS		:= -M accel=kvm:tcg -m 2G -net none -serial stdio -rtc base=utc -vga std -k en-us -cpu max

include libk/Makefile
include kernel/Makefile
include init/Makefile

headers:
	@mkdir -p hdd/usr/include
	@cp -RT libk/include hdd/usr/include

todo:
	@for file in $(shell find kernel libk -name '*.[chs]' -type f); do fgrep -H -e TODO -e FIXME $$file | sed -e 's/:\//: \//g' | sed -e 's/[[:space:]]/ /g'; done; true

clean: clean-kernel

distclean: clean-kernel clean-disk clean-font clean-libk clean-libc clean-libc-test clean-init clean-initrd

.PHONY: all clean test test-debug test-virtualbox
.SECONDARY:
.SUFFIXES:
