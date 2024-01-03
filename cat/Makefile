.SILENT:
	
CC := gcc
CFLAGS := -std=c11 -Wall -Wextra -Werror -D_POSIX_C_SOURCE=200809L

.PHONY: all clean

all: s21_cat
	
run: s21_cat
	$<

s21_cat: cat.o
	$(CC) -o $@ $^

cat.o: cat.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf *.o s21_cat ./test_cat/test_s21_cat.log