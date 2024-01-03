.SILENT:
	
CC := gcc
CFLAGS := -std=c11 -Wall -Wextra -Werror -D_POSIX_C_SOURCE=200809L -g

.PHONY: all clean

all: s21_grep
	
run: s21_grep
	$<

s21_grep: grep.o
	$(CC) -o $@ $^

grep.o: grep.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf *.o s21_grep ./test_grep/test_s21_grep.log ./test_grep/test_sys_grep.log