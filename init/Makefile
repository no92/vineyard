init/init: init/init.s
	@$(INFO) "INIT" "building $@"
	@$(YASM) -f elf32 -o $@.o $^
	@$(LD) -o $@ $@.o -ffreestanding -nostdlib

clean-init:
	@$(INFO) "CLEAN" "cleaning init"
	@rm -f init/init

.PHONY: clean-init
