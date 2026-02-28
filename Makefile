.PHONY: bin lib clean test

CC = clang
CFLAGS = -MMD -MP -Wall -Wextra -Wconversion -g -Iinclude

LIBRARY = build/libbrass.a
BIN = build/brassvm

SRC := $(shell find src -name "*.c")
OBJ := $(patsubst src/%.c,build/%.o,$(SRC))

SRCBIN := $(shell find srcbin -name "*.c")

TEST := $(shell find test -name "*.c")
TESTOBJ := $(patsubst test/%.c,build/test/%.o,$(TEST))
TESTBIN := $(TESTOBJ:.o=.exe)

bin: lib
	$(CC) $(SRCBIN) -Lbuild -lbrass -o $(BIN)

lib: $(LIBRARY)

$(LIBRARY): $(OBJ)
	ar rcs $@ $^

build/%.o: src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

build/test/%.o: test/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# link each test
$(TESTBIN): build/test/%.exe: build/test/%.o lib
	$(CC) $< -Lbuild -lbrass -o $@

test: $(TESTBIN)

clean:
	rm -rf build

-include $(OBJ:.o=.d)
