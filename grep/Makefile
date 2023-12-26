.SILENT:
	
CC := gcc
CFLAGS := -std=c11 -Wall -Wextra -Werror
LDFLAGS := -D_POSIX_C_SOURCE=200809L 

.PHONY: all clean

all: s21_grep
	
run: s21_grep
	$<

s21_grep: grep.o
	$(CC) $(LDFLAGS) -o $@ $^

grep.o: grep.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf *.o s21_grep
