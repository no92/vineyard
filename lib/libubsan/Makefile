LIBUBSAN		:= $(SYSROOT)/usr/lib/libubsan.a
LIBUBSAN_DIR	:= $(LIB)/libubsan
UBSAN_SRC		:= $(shell find $(LIBUBSAN_DIR) -name '*.c')
UBSAN_OBJ		:= $(addsuffix .o, $(UBSAN_SRC))
UBSAN_DEP		:= $(patsubst %.o,%.d,$(UBSAN_OBJ))

$(LIBUBSAN): $(UBSAN_OBJ)
	@$(INFO) "LD" "libubsan"
	@$(USER_AR) rcs $@ $^

$(LIBUBSAN_DIR)/%.c.o: $(LIBUBSAN_DIR)/%.c
	@$(INFO) "CC" $<
	@$(USER_GCC) $(CFLAGS_LIBK) -c -o $@ $<

clean-libubsan:
	@$(INFO) "CLEAN" "cleaning libubsan"
	@$(RM) $(UBSAN_OBJ) $(UBSAN_DEP)

-include $(UBSAN_DEP)

.PHONY: clean-libubsan
