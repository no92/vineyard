# make sure we're running bash
SHELL		:= /bin/bash

ISO			:= bin/vineyard.iso

# define 'all' as the default target
all: third-party $(ISO)

include third-party/Makefile

# define various tools
CC			:= $(CROSS_GCC)
LD			:= $(CROSS_LD)
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
CFLAGS		+= -c -ggdb3 -Ilibk -Iinclude -MD -nostdinc -pipe -std=gnu11
CFLAGS		+= -Wall -Wbad-function-cast -Wcast-align -Wconversion -Werror -Wextra -Wformat=2 -Wimplicit-fallthrough=3 -Winit-self -Winline -Wlogical-op -Wmissing-braces
CFLAGS		+= -Wmissing-declarations -Wmissing-field-initializers -Wmissing-prototypes -Wnested-externs -Wparentheses -Wpedantic -Wpointer-arith
CFLAGS		+= -Wredundant-decls -Wshadow -Wstrict-prototypes -Wswitch-default -Wswitch-enum -Wuninitialized -Wunreachable-code -Wunused -Wwrite-strings
LDFLAGS		:= -T build/kernel.ld -nostdlib
ASFLAGS		:= -f elf32
EMUARGS		:= -M accel=kvm:tcg -m 1024 -net none -serial stdio -rtc base=utc -vga std -k $(KEYBOARD)

include libk/Makefile
include kernel/Makefile

clean: clean-kernel

clean-deep: clean-kernel clean-font clean-libk clean-libc clean-libc-test

.PHONY: all clean clean-deep test test-debug test-virtualbox
.SECONDARY:
.SUFFIXES:
