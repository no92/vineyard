QEMU_URL			:= http://download.qemu-project.org/qemu-2.9.0.tar.xz
QEMU_DIR			:= third-party/qemu
QEMU_PATCHES		:= patches/qemu
QEMU				:= $(QEMU_DIR)/i386-softmmu/qemu-system-i386

YASM_URL			:= https://github.com/yasm/yasm
YASM_DIR			:= $(shell pwd)/third-party/yasm
YASM				:= $(YASM_DIR)/yasm

AUTOCONF_URL		:= https://ftp.gnu.org/gnu/autoconf/autoconf-2.64.tar.gz
AUTOCONF_DIR		:= $(shell pwd)/third-party/autoconf
AUTOCONF			:= $(AUTOCONF_DIR)/bin/autoconf

AUTOMAKE_URL		:= https://ftp.gnu.org/gnu/automake/automake-1.11.6.tar.gz
AUTOMAKE_DIR		:= $(shell pwd)/third-party/automake
AUTOMAKE			:= $(AUTOMAKE_DIR)/bin/automake

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

ACPICA_SCRIPT		:= third-party/scripts/acpica
ACPICA_VERSION		:= 20170831
ACPICA_URL			:= https://acpica.org/sites/acpica/files/acpica-unix-$(ACPICA_VERSION).tar.gz
ACPICA_DIR			:= third-party/acpica
ACPICA_ARCHIVE		:= $(ACPICA_DIR)/acpica.tar.gz
ACPICA_HEADERS		:= $(shell find include/acpi -name '*.h' -not -name 'acvineyard.h')
ACPICA				:= include/acpi/acpi.h

third-party: $(QEMU) $(YASM) $(AUTOCONF) $(AUTOMAKE) $(CROSS_GCC) $(USER_GCC) $(GRUB) $(UNI-VGA) $(ACPICA)
third-party-clean: qemu-clean yasm-clean autoconf-clean automake-clean gcc-clean user-gcc-clean grub-clean uni-vga-clean acpica-clean

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

$(YASM):
	$(INFO) "MAKE" "downloading yasm"
	mkdir -p $(YASM_DIR)
	git clone $(YASM_URL) $(YASM_DIR) -q
	$(INFO) "MAKE" "building yasm"
	bash -c 'cd $(YASM_DIR); cmake -DCMAKE_INSTALL_MESSAGE=NEVER -DCMAKE_INSTALL_PREFIX="$(YASM_DIR)" -Wno-dev .; make -C $(YASM_DIR) -s $(MUTE); make -C $(YASM_DIR) -s install $(MUTE)'

yasm-clean:
	$(RM) -R $(YASM_DIR)

$(AUTOCONF):
	$(INFO) "MAKE" "downloading autoconf"
	mkdir -p $(AUTOCONF_DIR)/src
	$(WGET) -O $(AUTOCONF_DIR)/autoconf.tar.gz $(AUTOCONF_URL)
	tar xzf $(AUTOCONF_DIR)/autoconf.tar.gz --strip-components=1 -C $(AUTOCONF_DIR)/src
	bash -c 'cd $(AUTOCONF_DIR); patch -p0 < $(AUTOMAKE_DIR)/../../patches/automake/automake.patch; src/configure --prefix="$(AUTOCONF_DIR)" --silent; make; make install -k || true'

autoconf-clean:
	$(RM) -R $(AUTOCONF_DIR)

$(AUTOMAKE):
	$(INFO) "MAKE" "downloading automake"
	mkdir -p $(AUTOMAKE_DIR)/src
	$(WGET) -O $(AUTOMAKE_DIR)/automake.tar.gz $(AUTOMAKE_URL)
	tar xzf $(AUTOMAKE_DIR)/automake.tar.gz --strip-components=1 -C $(AUTOMAKE_DIR)/src
	bash -c 'cd $(AUTOMAKE_DIR); src/configure --prefix="$(AUTOMAKE_DIR)" --silent; make; make install'

automake-clean:
	$(RM) -R $(AUTOMAKE_DIR)

$(CROSS_GCC): $(AUTOCONF) $(AUTOMAKE)
	chmod +x $(GCC_SCRIPT)
	PATH=$(AUTOCONF_DIR)/bin:$(AUTOMAKE_DIR)/bin:$(PERL_DIR):${PATH} $(GCC_SCRIPT) $(GCC_DIR) $(GCC_VER) $(BINUTILS_VER) $(GCC_TARGET)

gcc-clean:
	$(RM) -R $(GCC_DIR)

$(USER_GCC): $(AUTOCONF) $(AUTOMAKE)
	chmod +x $(USER_GCC_SCRIPT)
	PATH=$(AUTOCONF_DIR)/bin:$(AUTOMAKE_DIR)/bin:$(PERL_DIR):${PATH} $(USER_GCC_SCRIPT) $(USER_GCC_DIR) $(USER_GCC_VER) $(USER_BINUTILS_VER) $(USER_GCC_TARGET) $(SYSROOT)

user-gcc-clean:
	$(RM) -R $(USER_GCC_DIR)

$(GRUB):
	$(INFO) "MAKE" "downloading GRUB"
	mkdir -p $(GRUB_DIR)/src
	mkdir -p $(GRUB_DIR)/build
	$(WGET) -O $(GRUB_DIR)/grub.tar.gz $(GRUB_URL)
	$(INFO) "MAKE" "building GRUB"
	tar xzf $(GRUB_DIR)/grub.tar.gz --strip-components=1 -C $(GRUB_DIR)/src
	bash -c 'cd $(GRUB_DIR)/build; ../src/configure --prefix="$(GRUB_DIR)" --disable-nls --disable-dependency-tracking --silent --target=i386 CFLAGS="-Wimplicit-fallthrough=0 -Wno-error"; make -j4 -s; make install -s'
	bash -c 'cd $(GRUB_DIR)/bin; ./grub-mkfont -o $(GRUB_DIR)/share/grub/unicode.pf2 /usr/share/fonts/truetype/unifont/unifont.ttf'
	mkdir $(GRUB_DIR)/share/locale

grub-clean:
	$(RM) -R $(GRUB_DIR)

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
