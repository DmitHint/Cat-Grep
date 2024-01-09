.SILENT:
	
CC := gcc
CFLAGS := -std=c11 -Wall -Wextra -Werror -D_POSIX_C_SOURCE=200809L -g

.PHONY: all clean

all: my_grep tests clean

run: my_grep
	$<

tests: my_grep
	./test_grep/test_func_grep.sh

leak_tests: my_grep
	./test_grep/test_leak_grep.sh

my_grep: grep.o
	$(CC) -o $@ $^

grep.o: grep.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf *.o my_grep ./test_grep/test_my_grep.log ./test_grep/test_sys_grep.log