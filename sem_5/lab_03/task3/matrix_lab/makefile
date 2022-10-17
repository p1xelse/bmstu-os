СС := gcc
CFLAGS := -Iinc
LINKFLAGS := 
DEBUGFLAGS := -g3 --coverage
CHECK_FLAGS := -lcheck -lpthread -lrt
OBJS := out/matrix.o
OBJS_UNIT := out/check_matrix.o out/check_main.o
 
.NOTPARALLEL: debug release

release: CFLAGS += -DNDEBUG -g0
release : lastrelease app.exe

lastrelease:
	touch $@
	rm -f app.exe $(OBJS) out/main.o
	rm -f lastdebug

_MAKE_OUT_DIR_ := $(shell mkdir -p out)	

app.exe: $(OBJS) out/main.o
	$(СС) $(LINKFLAGS) -o $@ $^

unit_tests.exe: $(OBJS) $(OBJS_UNIT)
	$(СС) $(LINKFLAGS) $(CHECK_FLAGS) -o $@ $^

out/matrix.o: src/matrix.c inc/matrix.h inc/errors.h
	$(СС) $(CFLAGS) -o $@ -c $<

out/main.o: src/main.c inc/matrix.h inc/errors.h
	$(СС) $(CFLAGS) -o $@ -c $<

out/check_matrix.o: unit_tests/check_matrix.c inc/check_matrix.h
	$(СС) $(CFLAGS) -o $@ -c $<

out/check_main.o: unit_tests/check_main.c inc/check_matrix.h
	$(СС) $(CFLAGS) -o $@ -c $<

.PHONY: clean unit func debug release

unit: CFLAGS += $(DEBUGFLAGS) 
unit: LINKFLAGS += $(DEBUGFLAGS)
unit: lastdebug unit_tests.exe
	./unit_tests.exe
	gcov out/c*.gcno
	mv *.gcov out/
debug: CFLAGS += $(DEBUGFLAGS)
debug: LINKFLAGS += $(DEBUGFLAGS)
debug: lastdebug app.exe

lastdebug:
	touch $@
	rm -f app.exe $(OBJS) out/main.o
	rm -f lastrelease

func: debug
	cd func_tests; sh all_tests.sh
	cd out; gcov matrix.gcno main.gcno

clean:
	rm -rf out *.exe lastdebug lastrelease
