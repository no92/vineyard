QEMU_URL			:= http://download.qemu-project.org/qemu-2.9.0.tar.xz
QEMU_DIR			:= third-party/qemu
QEMU_PATCHES		:= patches/qemu
QEMU				:= $(QEMU_DIR)/i386-softmmu/qemu-system-i386

YASM_URL			:= https://github.com/yasm/yasm
YASM_DIR			:= $(shell pwd)/third-party/yasm
YASM				:= $(YASM_DIR)/yasm

GCC_TARGET			:= i686-elf
GCC_DIR				:= $(shell pwd)/third-party/i386/$(GCC_TARGET)-gcc
GCC_VER				:= 7.2.0
BINUTILS_VER		:= 2.29
GCC_SCRIPT			:= third-party/scripts/gcc
CROSS_GCC			:= $(GCC_DIR)/bin/$(GCC_TARGET)-gcc
CROSS_LD			:= $(GCC_DIR)/bin/$(GCC_TARGET)-ld
CROSS_AR			:= $(GCC_DIR)/bin/$(GCC_TARGET)-ar

USER_GCC_TARGET		:= i686-vineyard
SYSROOT				:= $(shell pwd)/hdd
USER_GCC_DIR		:= $(shell pwd)/third-party/i386/$(USER_GCC_TARGET)-gcc
USER_GCC_VER		:= 7.2.0
USER_BINUTILS_VER	:= 2.29
USER_GCC_SCRIPT		:= third-party/scripts/user-gcc
USER_GCC			:= $(USER_GCC_DIR)/bin/$(USER_GCC_TARGET)-gcc
USER_LD				:= $(USER_GCC_DIR)/bin/$(USER_GCC_TARGET)-ld
USER_AR				:= $(USER_GCC_DIR)/bin/$(USER_GCC_TARGET)-ar

GRUB_URL			:= http://alpha.gnu.org/gnu/grub/grub-2.02~rc2.tar.gz
GRUB_DIR			:= $(shell pwd)/third-party/i386/grub
GRUB				:= $(GRUB_DIR)/bin/grub-mkrescue

UNI-VGA_URL			:= http://www.inp.nsk.su/~bolkhov/files/fonts/univga/uni-vga.tgz
UNI-VGA_DIR			:= third-party/uni-vga
UNI-VGA				:= $(UNI-VGA_DIR)/u_vga16.bdf

BOCHS_URL			:= https://sourceforge.net/projects/bochs/files/bochs/2.6.9/bochs-2.6.9.tar.gz
BOCHS_DIR			:= $(shell pwd)/third-party/bochs
BOCHS_CONFIG		:= --enable-debugger --enable-debugger-gui --enable-readline --enable-disasm --enable-logging
BOCHS_CONFIG		+= --enable-cpu-level=6 --enable-smp --enable-fpu --enable-3dnow --enable-x86-64 --enable-avx --enable-x86-debugger --enable-all-optimizations
BOCHS_CONFIG		+= --enable-sb16=default --enable-ne2000 --enable-e1000 --enable-iodebug --enable-pci --enable-usb --enable-usb-ohci --enable-usb-ehci --enable-usb-xhci
BOCHS_CONFIG		+= --disable-plugins --disable-docbook
BOCHS_CONFIG		+= --with-sdl2
BOCHS				:= $(BOCHS_DIR)/bochs

ACPICA_VERSION		:= 20170831
ACPICA_URL			:= https://acpica.org/sites/acpica/files/acpica-unix-$(ACPICA_VERSION).tar.gz
ACPICA_DIR			:= third-party/acpica
ACPICA_ARCHIVE		:= $(ACPICA_DIR)/acpica.tar.gz
ACPICA_HEADERS		:= $(shell find include/acpi -name '*.h' -not -name 'acvineyard.h')
ACPICA				:= include/acpi/acpi.h

third-party: $(QEMU) $(YASM) $(CROSS_GCC) $(USER_GCC) $(GRUB) $(UNI-VGA) $(BOCHS) $(ACPICA)
third-party-clean: qemu-clean yasm-clean gcc-clean user-gcc-clean grub-clean uni-vga-clean bochs-clean acpica-clean

$(QEMU):
	@$(INFO) "MAKE" "downloading QEMU"
	@mkdir -p $(QEMU_DIR)
	@$(WGET) -O third-party/qemu.tar.xz $(QEMU_URL)
	@$(INFO) "MAKE" "building QEMU"
	@tar xJf third-party/qemu.tar.xz -C $(QEMU_DIR) --strip-components=1
	@patch $(QEMU_DIR)/block/vvfat.c < $(QEMU_PATCHES)/vvfat.c.patch
	@bash -c 'cd $(QEMU_DIR); ./configure --target-list=i386-softmmu,x86_64-softmmu --disable-docs --disable-curses --disable-gtk --disable-tools --disable-vnc; make V=0 -j4 -s'
	@rm third-party/qemu.tar.xz

qemu-clean:
	@$(RM) -R $(QEMU_DIR)

$(YASM):
	@$(INFO) "MAKE" "downloading yasm"
	@mkdir -p $(YASM_DIR)
	@git clone $(YASM_URL) $(YASM_DIR) -q
	@$(INFO) "MAKE" "building yasm"
	@bash -c 'cd $(YASM_DIR); cmake -DCMAKE_INSTALL_MESSAGE=NEVER -DCMAKE_INSTALL_PREFIX="$(YASM_DIR)" -Wno-dev .; make -C $(YASM_DIR) -s $(MUTE); make -C $(YASM_DIR) -s install $(MUTE)'

yasm-clean:
	@$(RM) -R $(YASM_DIR)

$(CROSS_GCC):
	@$(GCC_SCRIPT) $(GCC_DIR) $(GCC_VER) $(BINUTILS_VER) $(GCC_TARGET)

gcc-clean:
	@$(RM) -R $(GCC_DIR)

$(USER_GCC):
	@$(USER_GCC_SCRIPT) $(USER_GCC_DIR) $(USER_GCC_VER) $(USER_BINUTILS_VER) $(USER_GCC_TARGET) $(SYSROOT)

user-gcc-clean:
	@$(RM) -R $(USER_GCC_DIR)

$(GRUB):
	@$(INFO) "MAKE" "downloading GRUB"
	@mkdir -p $(GRUB_DIR)/src
	@mkdir -p $(GRUB_DIR)/build
	@$(WGET) -O $(GRUB_DIR)/grub.tar.gz $(GRUB_URL)
	@$(INFO) "MAKE" "building GRUB"
	@tar xzf $(GRUB_DIR)/grub.tar.gz --strip-components=1 -C $(GRUB_DIR)/src
	@bash -c 'cd $(GRUB_DIR)/build; ../src/configure --prefix="$(GRUB_DIR)" --disable-nls --disable-dependency-tracking --silent --target=i386; make -j4 -s; make install -s'
	@bash -c 'cd $(GRUB_DIR)/bin; ./grub-mkfont -o $(GRUB_DIR)/share/grub/unicode.pf2 /usr/share/fonts/truetype/unifont/unifont.ttf'
	@mkdir $(GRUB_DIR)/share/locale

grub-clean:
	@$(RM) -R $(GRUB_DIR)

$(UNI-VGA):
	@$(INFO) "MAKE" "getting uni-vga"
	@mkdir -p $(UNI-VGA_DIR)
	@$(WGET) $(UNI-VGA_URL) -O $(UNI-VGA_DIR)/uni-vga.tar.gz
	@tar xzf $(UNI-VGA_DIR)/uni-vga.tar.gz --strip-components=1 -C $(UNI-VGA_DIR)

uni-vga-clean:
	@$(RM) -R $(UNI-VGA_DIR)

$(BOCHS):
	@$(INFO) "MAKE" "getting bochs"
	@mkdir -p $(BOCHS_DIR)
	@wget $(BOCHS_URL) -O $(BOCHS_DIR)/bochs.tar.gz -q
	@tar xzf $(BOCHS_DIR)/bochs.tar.gz --strip-components=1 -C $(BOCHS_DIR)
	@bash -c 'cd $(BOCHS_DIR); ./configure $(BOCHS_CONFIG) --prefix="$(BOCHS_DIR)" -q; make -s; make install -s'

bochs-clean:
	@$(RM) -R $(BOCHS_DIR)

$(ACPICA):
	@$(INFO) "MAKE" "getting ACPICA"
	@mkdir -p $(ACPICA_DIR)
	@wget $(ACPICA_URL) -O $(ACPICA_ARCHIVE) -q
	@tar -xzf $(ACPICA_ARCHIVE) -C kernel/acpi/ acpica-unix-$(ACPICA_VERSION)/source/components/dispatcher --strip-components=3
	@tar -xzf $(ACPICA_ARCHIVE) -C kernel/acpi/ acpica-unix-$(ACPICA_VERSION)/source/components/events --strip-components=3
	@tar -xzf $(ACPICA_ARCHIVE) -C kernel/acpi/ acpica-unix-$(ACPICA_VERSION)/source/components/executer --strip-components=3
	@tar -xzf $(ACPICA_ARCHIVE) -C kernel/acpi/ acpica-unix-$(ACPICA_VERSION)/source/components/hardware --strip-components=3
	@tar -xzf $(ACPICA_ARCHIVE) -C kernel/acpi/ acpica-unix-$(ACPICA_VERSION)/source/components/namespace --strip-components=3
	@tar -xzf $(ACPICA_ARCHIVE) -C kernel/acpi/ acpica-unix-$(ACPICA_VERSION)/source/components/parser --strip-components=3
	@tar -xzf $(ACPICA_ARCHIVE) -C kernel/acpi/ acpica-unix-$(ACPICA_VERSION)/source/components/resources --strip-components=3
	@tar -xzf $(ACPICA_ARCHIVE) -C kernel/acpi/ acpica-unix-$(ACPICA_VERSION)/source/components/tables --strip-components=3
	@tar -xzf $(ACPICA_ARCHIVE) -C kernel/acpi/ acpica-unix-$(ACPICA_VERSION)/source/components/utilities --strip-components=3
	@patch -s -p0 < patches/acpica/components.patch
	@tar -xzf $(ACPICA_ARCHIVE) -C include/acpi/ acpica-unix-$(ACPICA_VERSION)/source/include/ --strip-components=3
	@patch -s -p0 < patches/acpica/headers.patch
	@$(RM) $(ACPICA_ARCHIVE)

acpica-clean:
	@$(RM) $(ACPICA_ARCHIVE)
	@$(RM) -R kernel/acpi/dispatcher kernel/acpi/events kernel/acpi/executer kernel/acpi/hardware kernel/acpi/namespace kernel/acpi/parser kernel/acpi/resources kernel/acpi/tables kernel/acpi/utilities
	@$(RM) $(ACPICA_HEADERS)

.PHONY: third-party third-party-clean qemu-clean yasm-clean gcc-clean user-gcc-clean grub-clean uni-vga-clean bochs-clean acpica-clean
