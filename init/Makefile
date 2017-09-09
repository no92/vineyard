init/init: $(LIBC) init/init.c
	@$(INFO) "INIT" "building $@"
	@$(USER_GCC) -o $@ init/init.c

clean-init:
	@$(INFO) "CLEAN" "cleaning init"
	@rm -f init/init

.PHONY: clean-init
