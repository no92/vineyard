INIT		:= init/init
INIT_SRC	:= $(shell find init -name *.c)
INIT_OBJ	:= $(addsuffix .o,$(INIT_SRC))
INIT_DEP	:= $(addsuffix .d,$(INIT_SRC))

$(INIT): $(LIBC) init/init.c
	@$(INFO) "INIT" "building $@"
	@$(USER_GCC) $(filter-out -fstack-protector-all, $(CFLAGS_LIBK)) -fno-stack-protector -o $@ init/init.c

clean-init:
	@$(INFO) "CLEAN" "cleaning init"
	@$(RM) $(INIT) $(INIT_OBJ) $(INIT_DEP)

-include $(INIT_DEP)

.PHONY: clean-init
