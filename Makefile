.SILENT:
	
CC := gcc
CFLAGS := -std=c11 -Wall -Wextra -Werror
LDFLAGS :=
COMMON_DIR := common
CAT_DIR := cat
GREP_DIR := grep
BUILD_DIR := build
BIN_DIR := bin

.PHONY: all clean

all: $(BIN_DIR)/cat $(BIN_DIR)/grep
	
run: $(BIN_DIR)/cat
	$<

$(BIN_DIR)/cat: $(BUILD_DIR)/cat.o
	$(CC) $(LDFLAGS) -o $@ $^

$(BIN_DIR)/grep: $(BUILD_DIR)/grep.o
	$(CC) $(LDFLAGS) -o $@ $^

$(BUILD_DIR)/cat.o: $(CAT_DIR)/cat.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILD_DIR)/grep.o: $(GREP_DIR)/grep.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf $(BUILD_DIR)/*.o $(BIN_DIR)/cat $(BIN_DIR)/grep

