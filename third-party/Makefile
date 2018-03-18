QEMU_URL			:= http://download.qemu-project.org/qemu-2.9.0.tar.xz
QEMU_DIR			:= third-party/qemu
QEMU_PATCHES		:= patches/qemu
QEMU				:= $(QEMU_DIR)/i386-softmmu/qemu-system-i386

UNI-VGA_URL			:= http://www.inp.nsk.su/~bolkhov/files/fonts/univga/uni-vga.tgz
UNI-VGA_DIR			:= third-party/uni-vga
UNI-VGA				:= $(UNI-VGA_DIR)/u_vga16.bdf

ACPICA_SCRIPT		:= third-party/scripts/acpica
ACPICA_VERSION		:= 20170831
ACPICA_URL			:= https://acpica.org/sites/acpica/files/acpica-unix-$(ACPICA_VERSION).tar.gz
ACPICA_DIR			:= third-party/acpica
ACPICA_ARCHIVE		:= $(ACPICA_DIR)/acpica.tar.gz
ACPICA_HEADERS		:= $(shell find include/acpi -name '*.h' -not -name 'acvineyard.h')
ACPICA				:= include/acpi/acpi.h

third-party: $(QEMU) $(UNI-VGA) $(ACPICA)
third-party-clean: qemu-clean uni-vga-clean acpica-clean

$(QEMU):
	$(INFO) "MAKE" "downloading QEMU"
	mkdir -p $(QEMU_DIR)
	$(WGET) -O third-party/qemu.tar.xz $(QEMU_URL)
	$(INFO) "MAKE" "building QEMU"
	tar xJf third-party/qemu.tar.xz -C $(QEMU_DIR) --strip-components=1
	patch $(QEMU_DIR)/block/vvfat.c < $(QEMU_PATCHES)/vvfat.c.patch
	bash -c 'cd $(QEMU_DIR); ./configure --target-list=i386-softmmu,x86_64-softmmu --disable-docs --disable-curses --disable-gtk --disable-tools --disable-vnc; make V=0 -j4 -s'
	rm third-party/qemu.tar.xz

qemu-clean:
	$(RM) -R $(QEMU_DIR)

$(UNI-VGA):
	$(INFO) "MAKE" "getting uni-vga"
	mkdir -p $(UNI-VGA_DIR)
	$(WGET) $(UNI-VGA_URL) -O $(UNI-VGA_DIR)/uni-vga.tar.gz
	tar xzf $(UNI-VGA_DIR)/uni-vga.tar.gz --strip-components=1 -C $(UNI-VGA_DIR)

uni-vga-clean:
	$(RM) -R $(UNI-VGA_DIR)

$(ACPICA):
	chmod +x $(ACPICA_SCRIPT)
	$(ACPICA_SCRIPT) $(ACPICA_DIR) $(ACPICA_URL) $(ACPICA_ARCHIVE) $(ACPICA_VERSION)

acpica-clean:
	$(RM) $(ACPICA_ARCHIVE)
	$(RM) -R kernel/acpi/dispatcher kernel/acpi/events kernel/acpi/executer kernel/acpi/hardware kernel/acpi/namespace kernel/acpi/parser kernel/acpi/resources kernel/acpi/tables kernel/acpi/utilities
	$(RM) $(ACPICA_HEADERS)

.PHONY: third-party third-party-clean qemu-clean yasm-clean autoconf-clean automake-clean gcc-clean user-gcc-clean grub-clean uni-vga-clean acpica-clean
