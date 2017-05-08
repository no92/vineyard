# make sure we're running bash
SHELL		:= /bin/bash

# define some output file names
ISO			:= bin/vineyard.iso
KERNEL		:= bin/vineyard
HDD			:= hdd
CONFIG		:= $(HDD)/boot/grub/grub.cfg
KERNEL_ISO	:= $(HDD)/boot/vineyard

# define 'all' as the default target
all: third-party $(ISO)

# find the kernel sources that we compile
UNI-VGA_C	:= kernel/gfx/font.c

KERNEL_SRC	:= $(shell find kernel libk -name '*.[sc]' -type f)
KERNEL_SRC	+= $(UNI-VGA_C)
KERNEL_OBJ	:= $(addsuffix .o,$(KERNEL_SRC))
KERNEL_DEP	:= $(addsuffix .d,$(KERNEL_SRC))

# if not passed to make, set these default resolutions in the multiboot2 header
MB2_WIDTH	?= 1280
MB2_HEIGHT	?= 720

include third-party/Makefile

# define various tools
CC			:= $(CROSS_GCC)
LD			:= $(CROSS_LD)
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
CFLAGS		:= -Wall -Wbad-function-cast -Wcast-align -Wconversion -Werror -Wextra -Winit-self -Winline -Wlogical-op -Wmissing-braces
CFLAGS		+= -Wmissing-declarations -Wmissing-field-initializers -Wmissing-prototypes -Wnested-externs -Wparentheses -Wpointer-arith
CFLAGS		+= -Wredundant-decls -Wshadow -Wstrict-prototypes -Wswitch-default -Wswitch-enum -Wuninitialized -Wunreachable-code -Wunused -Wwrite-strings
CFLAGS		+= -MD -pipe -std=gnu11 -pedantic -nostdinc -ffreestanding -finline-functions -fno-omit-frame-pointer -Ilibk -Iinclude -c -ggdb3
LDFLAGS		:= -T build/kernel.ld -nostdlib
ASFLAGS		:= -f elf32
EMUARGS		:= -M accel=kvm:tcg -m 1024 -net none -serial stdio -rtc base=utc -vga std -k $(KEYBOARD)

%.c.o: %.c
	@$(INFO) "CC" $<
	@$(CC) $(CFLAGS) -o $@ $<

%.s.o: %.s
	@$(INFO) "AS" $<
	@$(AS) $(ASFLAGS) -o $@ $<

kernel/init/boot.s.o: ASFLAGS += -DMB2_WIDTH=$(MB2_WIDTH) -DMB2_HEIGHT=$(MB2_HEIGHT)

kernel/gfx/font.c: $(UNI-VGA)
	@$(INFO) "BDF-TO-C" "$@"
	@util/bdf-to-c $< > $@

$(KERNEL): $(KERNEL_OBJ)
	@$(INFO) "LD" $@
	@$(LD) $(LDFLAGS) -o $@ $^

$(ISO): $(KERNEL)
	@$(INFO) "DISK" $@
	@$(MKDIR) $(dir $(CONFIG))
	@$(CP) build/grub.cfg $(CONFIG)
	@$(CP) $(KERNEL) $(KERNEL_ISO)
	@$(GRUB) -o $@ $(HDD) 2> .error.log; if grep "FAIL" .error.log -q; then $(ERROR) "GRUB" "building $@ failed; output of $(GRUB):"; cat .error.log; rm .error.log; $(ERROR) "GRUB" "end of output"; exit 1; else rm .error.log; fi;
	@$(GRUB_DIR)/bin/grub-file --is-x86-multiboot2 $< || $(ERROR) "DISK" "$@ is not a valid multiboot2 kernel"

clean:
	@$(INFO) "CLEAN" "cleaning build files"
	@$(RM) $(KERNEL_OBJ) $(KERNEL) $(KERNEL_ISO) $(ISO)

clean-font:
	@$(RM) $(UNI-VGA_C)

test: all
	@$(INFO) "QEMU" "running emulator"
	@$(EMU) $(EMUARGS) -cdrom $(ISO)

test-debug: all
	@$(INFO) "QEMU" "running emulator"
	@$(EMU) $(EMUARGS) -cdrom $(ISO) -S -s

test-virtualbox: all
	@$(INFO) "VBOX" "running emulator"
	@$(VBOX) vineyard $(ISO)

.PHONY: all clean test
.SECONDARY:
.SUFFIXES:

-include $(KERNEL_DEPS)
