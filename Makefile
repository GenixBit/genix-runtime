CC ?= cc
CFLAGS ?= -std=c11 -Wall -Wextra -Werror -Iinclude
BUILD_DIR := build

.PHONY: all test clean

all: $(BUILD_DIR)/libgenix_runtime.a

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/runtime.o: src/runtime.c include/genix/runtime.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c src/runtime.c -o $@

$(BUILD_DIR)/libgenix_runtime.a: $(BUILD_DIR)/runtime.o
	ar rcs $@ $^

$(BUILD_DIR)/runtime_test: tests/runtime_test.c src/runtime.c include/genix/runtime.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) tests/runtime_test.c src/runtime.c -o $@

test: $(BUILD_DIR)/runtime_test
	./$(BUILD_DIR)/runtime_test

clean:
	rm -rf $(BUILD_DIR)
