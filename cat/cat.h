#ifndef CAT_H
#define CAT_H

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct arguments {
  int b, n, s, E, T, v;
} arguments;

arguments parse_arguments(int argc, char **argv);

char change_v(char ch);

int show_file(const arguments *args, FILE *file, int *line_count);

int output(const arguments *args, int argc, char **argv);

void print_line(arguments args, char *line, int len);

#endif