LIBK		:= bin/libk.a
LIBC		:= hdd/usr/lib/libc.a
LIBC_DIR	:= $(LIB)/libc
LIBK_SRC	:= $(shell find $(LIBC_DIR) -name '*.[cs]' -type f -not -name 'crt[0ni].s')
LIBK_SRC	+= $(LIBC_DIR)/ubsan/ubsan.c
LIBK_OBJ	:= $(LIBK_SRC:.c=.libk.c.o)
LIBK_OBJ	:= $(LIBK_OBJ:.s=.libk.s.o)
LIBC_OBJ	:= $(LIBK_SRC:.c=.libc.c.o)
LIBC_OBJ	:= $(LIBC_OBJ:.s=.libc.s.o)
LIBK_DEP	:= $(LIBK_SRC:.c=.libk.c.d)
LIBK_DEP	:= $(LIBK_DEP:.s=.libk.s.d)
LIBC_DEP	:= $(LIBK_SRC:.c=.libc.c.d)
LIBC_DEP	:= $(LIBC_DEP:.s=.libc.s.d)

LIBC_TESTS	:= $(LIBC_OBJ:.libc.c.o=.test)

CFLAGS_LIBK	:= $(CFLAGS) $(CFLAGS_GCC)

libc-crt: hdd/usr/lib/crt0.o hdd/usr/lib/crti.o hdd/usr/lib/crtn.o

$(LIBC_DIR)/ubsan/ubsan.c: lib/libubsan/ubsan.c $(LIBC_DIR)/include/ubsan.h
	@mkdir -p $(dir $@)
	@cp $< $@

$(LIBC_DIR)/include/ubsan.h: lib/libubsan/include/ubsan.h
	@cp $< $@

%.libk.c.o: %.c
	@$(INFO) "CC" $<
	@$(CC) $(CFLAGS_LIBK) -c -o $@ $< -D__libk

%.libk.s.o: %.s
	@$(INFO) "AS" $<
	@$(AS) $(ASFLAGS) -o $@ $<

%.libc.c.o: %.c
	@$(INFO) "CC" $<
	@$(USER_GCC) $(filter-out -fstack-protector-all, $(CFLAGS_LIBK)) -c -o $@ $<

%.libc.s.o: %.s
	@$(INFO) "AS" $<
	@$(AS) $(ASFLAGS) -o $@ $<

hdd/usr/lib/crt%.o: $(LIBC_DIR)/crt%.s
	@$(INFO) "AS" $<
	@$(AS) -o $@ $< -f elf32

$(LIBK): $(LIBK_OBJ)
	@$(INFO) "AR" $@
	@$(MKDIR) $(dir $@)
	@$(AR) rcs $@ $^

$(LIBC): $(LIBC_OBJ) libc-crt $(LIBUBSAN)
	@$(INFO) "AR" $@
	@$(MKDIR) $(dir $@)
	@$(USER_AR) rcs $@ $(LIBC_OBJ)

%.test: %.c
	@$(INFO) "CC" $<
	@gcc $(filter-out -Wrestrict,$(CFLAGS_LIBK)) -o $@ $< -DUNIT -Ilibk -Lbin -lc

clean-libk:
	@$(INFO) "CLEAN" "cleaning libk"
	@$(RM) $(LIBK_OBJ) $(LIBK) $(LIBK_DEP)

clean-libc: clean-headers
	@$(INFO) "CLEAN" "cleaning libc"
	@$(RM) $(LIBC_OBJ) $(LIBC) $(LIBC_DEP) hdd/usr/lib/crt[0ni].o $(LIBC_DIR)/ubsan/ubsan.c $(LIBC_DIR)/include/ubsan.h

clean-libc-test:
	@$(INFO) "CLEAN" "cleaning libc tests"
	@$(RM) $(LIBC_TESTS) $(LIBC_DEP)

test-libc: $(LIBC_TESTS)
	@for test in $^; do \
		$(INFO) "TEST" "$$test"; \
		./$$test 2> .test.log || $(ERROR) "TEST" "$$test failed"; if grep -v bash .test.log -q; then cat .test.log; rm .test.log; else rm .test.log; fi; \
	done 2> /dev/null

-include $(LIBK_DEP)

.PHONY: libc-crt clean-libk clean-libc clean-libc-test test-libc
