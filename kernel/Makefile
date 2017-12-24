# define some output file names
KERNEL			:= bin/vineyard
INITRD			:= bin/initrd

VMWARE_CONFIG	:= conf/vineyard.vmx

HDD				:= hdd
PXE_CONFIG		:= $(HDD)/boot/i386-pc/grub.cfg
GRUB_CONFIG		:= $(HDD)/boot/grub/grub.cfg
KERNEL_ISO		:= $(HDD)/boot/vineyard
INITRD_ISO		:= $(HDD)/boot/initrd

# if not passed to make, set these default resolutions in the multiboot2 header
MB2_WIDTH		?= 1280
MB2_HEIGHT		?= 720

CFLAGS_KERN		:= $(CFLAGS) $(CFLAGS_GCC)
CFLAGS_LINT		:= -c -D__libk -target i386-pc-none-elf -Wno-language-extension-token -Wno-unreachable-code
CFLAGS_ACPI		:= $(filter-out -fsanitize=undefined,$(CFLAGS))
CFLAGS_ACPI		+= -Wno-unused-parameter -Wno-discarded-qualifiers -Wno-nested-externs -Wno-conversion -Wno-format
CFLAGS_ACPI		+= -Wno-missing-prototypes -Wno-missing-declarations -Wno-implicit-fallthrough -Wno-suggest-attribute=noreturn
CFLAGS_ACPI		+= -Wno-unused-but-set-variable -Wno-unused-function

# find the kernel sources that we compile
KERNEL_SRC		:= $(shell find kernel -name '*.[sc]' -type f -not -name 'crt[in].s')
KERNEL_SRC		:= $(filter-out kernel/gfx/font.c,$(KERNEL_SRC)) kernel/gfx/font.c
KERNEL_DEP		:= $(addsuffix .d,$(KERNEL_SRC))

KERNEL_OBJ		:= kernel/init/crti.s.o
KERNEL_OBJ		+= $(shell $(CC) $(CFLAGS_KERN) -print-file-name=crtbegin.o)
KERNEL_OBJ		+= $(addsuffix .o,$(KERNEL_SRC))
KERNEL_OBJ		+= $(LIBK)
KERNEL_OBJ		+= $(shell $(CC) $(CFLAGS_KERN) -print-file-name=crtend.o)
KERNEL_OBJ		+= kernel/init/crtn.s.o

%.c.o: %.c
	$(INFO) "CC" $<
	$(CC) $(CFLAGS_KERN) -c -o $@ $< -D__libk

%.s.o: %.s
	$(INFO) "AS" $<
	$(AS) $(ASFLAGS) -o $@ $<

kernel/init/boot.s.o: ASFLAGS += -DMB2_WIDTH=$(MB2_WIDTH) -DMB2_HEIGHT=$(MB2_HEIGHT)
kernel/init/ssp.c.o: CFLAGS_KERN := $(filter-out -fstack-protector-all,$(CFLAGS_KERN)) -fno-stack-protector
kernel/acpi/%.c.o: CFLAGS_KERN := $(CFLAGS_ACPI)

kernel/gfx/font.c: $(UNI-VGA)
	$(INFO) "BDF-TO-C" "$@"
	util/bdf-to-c $< > $@

$(KERNEL): $(ACPICA) $(KERNEL_OBJ)
	$(INFO) "LD" $@
	$(MKDIR) -p $(dir $@)
	$(LD) -o $@ $(KERNEL_OBJ) $(LDFLAGS)

$(INITRD): initrd/* $(INIT)
	$(INFO) "INITRD" $@
	cp $(INIT) initrd/init
	cd initrd; tar -cf ../bin/initrd *

$(ISO): $(KERNEL) $(INITRD)
	$(INFO) "DISK" $@
	$(MKDIR) $(dir $(GRUB_CONFIG))
	$(CP) build/grub.cfg $(GRUB_CONFIG)
	$(CP) $(KERNEL) $(KERNEL_ISO)
	$(CP) $(INITRD) $(INITRD_ISO)
	$(GRUB) -o $@ $(HDD) 2> .error.log; if grep "FAIL" .error.log -q; then $(ERROR) "GRUB" "building $@ failed; output of $(GRUB):"; cat .error.log; rm .error.log; $(ERROR) "GRUB" "end of output"; exit 1; else rm .error.log; fi;
	$(GRUB_DIR)/bin/grub-file --is-x86-multiboot2 $< || $(ERROR) "DISK" "$@ is not a valid multiboot2 kernel"
	$(GRUB_DIR)/bin/grub-mknetdir --net-directory=$(HDD)/boot --subdir grub > /dev/null
	cd $(HDD); ln -sf boot/grub/i386-pc/ i386-pc

clean-kernel:
	$(INFO) "CLEAN" "cleaning kernel tree"
	$(RM) $(addsuffix .o,$(KERNEL_SRC)) $(KERNEL) $(KERNEL_ISO) $(ISO) $(KERNEL_DEP) kernel/init/crti.s.o kernel/init/crtn.s.o

clean-font:
	$(INFO) "CLEAN" "cleaning font"
	$(RM) kernel/gfx/font.c

clean-disk:
	$(INFO) "CLEAN" "cleaning hard drive image"
	$(RM) -r $(HDD)/boot

clean-initrd:
	$(INFO) "CLEAN" "cleaning initrd"
	rm -f $(INITRD)

test: test-qemu

test-qemu: all
	$(INFO) "QEMU" "running emulator"
	$(EMU) $(EMUARGS) -cdrom $(ISO)

test-debug: all
	$(INFO) "QEMU" "running emulator"
	$(EMU) $(EMUARGS) -cdrom $(ISO) -S -s

test-virtualbox: all
	$(INFO) "VBOX" "running emulator"
	$(VBOX) vineyard $(ISO)

test-vmware: all
	if ! command -v vmrun &> /dev/null; then $(ERROR) "VMWARE" "VMWare Workstation is not installed"; fi
	$(INFO) "VMWARE" "running emulator"
	$(VMWARE) $(VMWARE_CONFIG)
	rm -f conf/vineyard.vmxf conf/vineyard.vmsd

test-bochs: all
	$(INFO) "BOCHS" "running emulator"
	$(BOCHS) -f conf/bochsrc -rc conf/bochsinit -q || true

-include $(KERNEL_DEP)

.PHONY: clean-kernel clean-font clean-disk clean-initrd test test-qemu test-debug test-virtualbox test-vmware test-bochs
