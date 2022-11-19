SRC_C=$(wildcard src/*.c)
OUT_C=$(patsubst src/%.c,build/%,$(SRC_C))
SRC_H=$(wildcard src/*.h)

SRC_TEST=$(wildcard test/*.c)
DEV_TEST=$(patsubst test/%.c,build/%.dev.test,$(SRC_TEST))
OPT_TEST=$(patsubst test/%.c,build/%.opt.test,$(SRC_TEST))
RUN=0
DEBUG=0
VALGRIND=0


dev: build $(DEV_TEST)
opt: build $(OPT_TEST)

run: RUN=1
run: dev

debug: dev
	gdb -ex run build/$$(ls build -1t | head -1)

valgrind: VALGRIND=1
valgrind: dev

build/%.dev.test: test/%.c src/%.h test/test.h
	gcc -g -Isrc -Wall $< -o $@
	@test $(RUN) -eq 0 || $@
	@test $(VALGRIND) -eq 0 || valgrind $@

build/%.opt.test: test/%.c src/%.h test/test.h
	gcc -O3 -Isrc $< -o $@


build:
	mkdir build

clean:
	rm build/*
