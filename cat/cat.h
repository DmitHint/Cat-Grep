#ifndef CAT_H
#define CAT_H

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct arguments {
  int b, n, s, E, T, v;
} arguments;

void printNumLeft(int *counter, int isNewLine);

char change_v(char ch);

int show_file(arguments *args, FILE *file);

arguments parse_arguments(int argc, char **argv, int *idx);

int output(arguments *args, int argc, char **argv);

#endif
