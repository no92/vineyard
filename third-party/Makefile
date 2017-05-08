QEMU_URL		:= http://download.qemu-project.org/qemu-2.9.0.tar.xz
QEMU_DIR		:= third-party/qemu
QEMU_PATCHES	:= patches/qemu
QEMU			:= $(QEMU_DIR)/i386-softmmu/qemu-system-i386

YASM_URL		:= https://github.com/yasm/yasm
YASM_DIR		:= $(shell pwd)/third-party/yasm
YASM			:= $(YASM_DIR)/yasm

GCC_DIR			:= $(shell pwd)/third-party/gcc
GCC_TARGET		:= i686-elf
GCC_URL			:= https://ftp.gnu.org/gnu/gcc/gcc-7.1.0/gcc-7.1.0.tar.gz
BINUTILS_URL	:= https://ftp.gnu.org/gnu/binutils/binutils-2.28.tar.gz
CROSS_GCC		:= $(GCC_DIR)/bin/$(GCC_TARGET)-gcc
CROSS_LD		:= $(GCC_DIR)/bin/$(GCC_TARGET)-ld
CROSS_AR		:= $(GCC_DIR)/bin/$(GCC_TARGET)-ar

GRUB_URL		:= http://alpha.gnu.org/gnu/grub/grub-2.02~rc2.tar.gz
GRUB_DIR		:= $(shell pwd)/third-party/grub
GRUB			:= $(GRUB_DIR)/bin/grub-mkrescue

UNI-VGA_URL		:= http://www.inp.nsk.su/~bolkhov/files/fonts/univga/uni-vga.tgz
UNI-VGA_DIR		:= third-party/uni-vga
UNI-VGA			:= $(UNI-VGA_DIR)/u_vga16.bdf

third-party: $(QEMU) $(YASM) $(CROSS_GCC) $(GRUB) $(UNI-VGA)
third-party-clean: qemu-clean yasm-clean gcc-clean grub-clean uni-vga-clean

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
	@$(INFO) "MAKE" "downloading gcc"
	@mkdir -p $(GCC_DIR)/gcc
	@$(WGET) -O $(GCC_DIR)/gcc.tar.gz $(GCC_URL)
	@$(INFO) "MAKE" "downloading binutils"
	@mkdir -p $(GCC_DIR)/binutils
	@$(WGET) -O $(GCC_DIR)/binutils.tar.gz $(BINUTILS_URL)
	@$(INFO) "MAKE" "building gcc & binutils"
	@tar xzf $(GCC_DIR)/gcc.tar.gz --strip-components=1 -C $(GCC_DIR)/gcc
	@tar xzf $(GCC_DIR)/binutils.tar.gz --strip-components=1 -C $(GCC_DIR)/binutils
	@bash -c 'cd $(GCC_DIR)/gcc; ./contrib/download_prerequisites' $(MUTE)
	@mkdir -p $(GCC_DIR)/binutils-build
	@bash -c 'cd $(GCC_DIR)/binutils-build; ../binutils/configure --quiet --target=$(GCC_TARGET) --prefix="$(GCC_DIR)" --with-sysroot --disable-nls --disable-werror; make -s; make install -s; make clean -s'
	@mkdir -p $(GCC_DIR)/gcc-build
	@bash -c 'cd $(GCC_DIR)/gcc-build; ../gcc/configure --quiet --target=$(GCC_TARGET) --prefix="$(GCC_DIR)" --disable-nls --enable-languages=c --without-headers; make -j4 all-gcc -s; make -j4 all-target-libgcc -s; make install-gcc -s; make install-target-libgcc -s; make clean -s'

gcc-clean:
	@$(RM) -R $(GCC_DIR)

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

.PHONY: third-party third-party-clean qemu-clean yasm-clean gcc-clean grub-clean uni-vga-clean
