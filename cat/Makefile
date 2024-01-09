.SILENT:
	
CC := gcc
CFLAGS := -std=c11 -Wall -Wextra -Werror -D_POSIX_C_SOURCE=200809L

.PHONY: all clean

all: my_cat
	
run: my_cat
	$<

tests: my_cat
	./test_cat/test_func_cat.sh

leak_tests: my_cat
	./test_cat/test_leak_cat.sh

my_cat: cat.o
	$(CC) -o $@ $^

cat.o: cat.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf *.o my_cat test_cat/test_my_cat.log