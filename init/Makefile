init/init: $(LIBC) init/init.c
	@$(INFO) "INIT" "building $@"
	@$(USER_GCC) $(filter-out -fstack-protector-all, $(CFLAGS_LIBK)) -o $@ init/init.c

clean-init:
	@$(INFO) "CLEAN" "cleaning init"
	@rm -f init/init

.PHONY: clean-init
