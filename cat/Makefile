.SILENT:
	
CC := gcc
CFLAGS := -std=c11 -Wall -Wextra -Werror
LDFLAGS :=

.PHONY: all clean

all: s21_cat
	
run: s21_cat
	$<

s21_cat: cat.o
	$(CC) $(LDFLAGS) -o $@ $^

cat.o: cat.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf *.o s21_cat
