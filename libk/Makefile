LIBK		:= bin/libk.a
LIBC		:= bin/libc.a
LIBK_SRC	:= $(shell find libk -name '*.[sc]' -type f)
LIBK_OBJ	:= $(LIBK_SRC:.c=.libk.c.o)
LIBC_OBJ	:= $(LIBK_SRC:.c=.libc.c.o)
LIBK_DEP	:= $(LIBK_SRC:.c=.libk.c.d)
LIBC_DEP	:= $(LIBK_SRC:.c=.d)

LIBC_TESTS	:= $(LIBC_OBJ:.libc.c.o=.test)

CFLAGS_LIBK	:= $(CFLAGS) $(CFLAGS_GCC)

%.libk.c.o: %.c
	@$(INFO) "CC" $<
	@$(CC) $(CFLAGS_LIBK) -c -o $@ $< -D__libk

%.libc.c.o: %.c
	@$(INFO) "CC" $<
	@gcc $(filter-out -Wrestrict,$(CFLAGS_LIBK)) -c -o $@ $<

$(LIBK): $(LIBK_OBJ)
	@$(INFO) "AR" $@
	@$(AR) rcs $@ $^

$(LIBC): $(LIBC_OBJ)
	@$(INFO) "AR" $@
	@$(AR) rcs $@ $^

%.test: %.c
	@$(INFO) "CC" $<
	@gcc $(filter-out -Wrestrict,$(CFLAGS_LIBK)) -o $@ $< -DUNIT -Ilibk -Lbin -lc

clean-libk:
	@$(INFO) "CLEAN" "cleaning libk"
	@$(RM) $(LIBK_OBJ) $(LIBK) $(LIBK_DEP)

clean-libc:
	@$(INFO) "CLEAN" "cleaning libc"
	@$(RM) $(LIBC_OBJ) $(LIBC) $(LIBC_DEP)

clean-libc-test:
	@$(INFO) "CLEAN" "cleaning libc tests"
	@$(RM) $(LIBC_TESTS) $(LIBC_DEP)

test-libc: $(LIBC_TESTS)
	@for test in $^; do \
		$(INFO) "TEST" "$$test"; \
		./$$test 2> .test.log || $(ERROR) "TEST" "$$test failed"; if grep -v bash .test.log -q; then cat .test.log; rm .test.log; else rm .test.log; fi; \
	done 2> /dev/null

test-libk-static:
	@$(INFO) "OCLINT" "running static analysis on libk"
	@$(OCLINT) $(LIBK_SRC) -- $(CFLAGS) -c || true

.PHONY: clean-libk clean-libc clean-libc-test test-libc
