LIBUBSAN		:= $(SYSROOT)/usr/lib/libubsan.a
LIBUBSAN_DIR	:= $(LIB)/libubsan
UBSAN_SRC		:= $(shell find $(LIBUBSAN_DIR) -name '*.c')
UBSAN_OBJ		:= $(addsuffix .o, $(UBSAN_SRC))

$(LIBUBSAN): $(UBSAN_OBJ)
	@$(INFO) "LD" "libubsan"
	@$(USER_AR) rcs $@ $^

$(LIBUBSAN_DIR)/%.c.o: $(LIBUBSAN_DIR)/%.c
	@$(INFO) "CC" $<
	@$(USER_GCC) $(CFLAGS_LIBK) -c -o $@ $<
