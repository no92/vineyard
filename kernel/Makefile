# define some output file names
KERNEL		:= bin/vineyard
HDD			:= hdd
GRUB_CONFIG	:= $(HDD)/boot/grub/grub.cfg
KERNEL_ISO	:= $(HDD)/boot/vineyard

# if not passed to make, set these default resolutions in the multiboot2 header
MB2_WIDTH	?= 1280
MB2_HEIGHT	?= 720

CFLAGS_KERN	:= $(CFLAGS) $(CFLAGS_GCC)

# find the kernel sources that we compile
KERNEL_SRC	:= $(shell find kernel -name '*.[sc]' -type f -not -name 'crt[in].s')
KERNEL_SRC	:= $(filter-out kernel/gfx/font.c,$(KERNEL_SRC)) kernel/gfx/font.c
KERNEL_DEP	:= $(addsuffix .d,$(KERNEL_SRC))

KERNEL_OBJ	:= kernel/init/crti.s.o
KERNEL_OBJ	+= $(shell $(CC) $(CFLAGS_KERN) -print-file-name=crtbegin.o)
KERNEL_OBJ	+= $(addsuffix .o,$(KERNEL_SRC))
KERNEL_OBJ	+= $(LIBK)
KERNEL_OBJ	+= $(shell $(CC) $(CFLAGS_KERN) -print-file-name=crtend.o)
KERNEL_OBJ	+= kernel/init/crtn.s.o

%.c.o: %.c
	@$(INFO) "CC" $<
	@$(CC) $(CFLAGS_KERN) -c -o $@ $< -D__libk

%.s.o: %.s
	@$(INFO) "AS" $<
	@$(AS) $(ASFLAGS) -o $@ $<

kernel/init/boot.s.o: ASFLAGS += -DMB2_WIDTH=$(MB2_WIDTH) -DMB2_HEIGHT=$(MB2_HEIGHT)
kernel/init/ssp.c.o: CFLAGS_KERN := $(filter-out -fstack-protector-all,$(CFLAGS_KERN)) -fno-stack-protector

kernel/gfx/font.c: $(UNI-VGA)
	@$(INFO) "BDF-TO-C" "$@"
	@util/bdf-to-c $< > $@

$(KERNEL): $(KERNEL_OBJ)
	@$(INFO) "LD" $@
	@$(LD) -o $@ $(KERNEL_OBJ) $(LDFLAGS)

$(ISO): $(KERNEL)
	@$(INFO) "DISK" $@
	@$(MKDIR) $(dir $(GRUB_CONFIG))
	@$(CP) build/grub.cfg $(GRUB_CONFIG)
	@$(CP) $(KERNEL) $(KERNEL_ISO)
	@$(GRUB) -o $@ $(HDD) 2> .error.log; if grep "FAIL" .error.log -q; then $(ERROR) "GRUB" "building $@ failed; output of $(GRUB):"; cat .error.log; rm .error.log; $(ERROR) "GRUB" "end of output"; exit 1; else rm .error.log; fi;
	@$(GRUB_DIR)/bin/grub-file --is-x86-multiboot2 $< || $(ERROR) "DISK" "$@ is not a valid multiboot2 kernel"

clean-kernel:
	@$(INFO) "CLEAN" "cleaning kernel tree"
	@$(RM) $(addsuffix .o,$(KERNEL_SRC)) $(KERNEL) $(KERNEL_ISO) $(ISO) $(KERNEL_DEP) kernel/init/crti.s.o kernel/init/crtn.s.o

clean-font:
	@$(INFO) "CLEAN" "cleaning font"
	@$(RM) kernel/gfx/font.c

test: all
	@$(INFO) "QEMU" "running emulator"
	@$(EMU) $(EMUARGS) -cdrom $(ISO)

test-debug: all
	@$(INFO) "QEMU" "running emulator"
	@$(EMU) $(EMUARGS) -cdrom $(ISO) -S -s

test-virtualbox: all
	@$(INFO) "VBOX" "running emulator"
	@$(VBOX) vineyard $(ISO)

test-kernel:
	@$(INFO) "OCLINT" "running static analysis on kernel"
	@$(OCLINT) $(filter-out %.s,$(KERNEL_SRC)) -- $(CFLAGS) -c -D__libk -target i386-pc-none-elf

-include $(KERNEL_DEP)

.PHONY: clean-kernel clean-font test test-debug test-virtualbox
