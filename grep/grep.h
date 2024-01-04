#ifndef GREP_H
#define GREP_H

#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct arguments {
  int e, i, v, c, l, n, h, s, f, o;

  char **patterns;
  int patterns_len;

} arguments;

void pattern_add(arguments *opts, const char *pattern);

void regs_from_file(arguments *opts, const char *reg_path);

arguments parse_arguments(int argc, char **argv);

void output_line(const char *line, int len);

int check_line(regex_t *re_list, arguments arg, char *line);

void process_file(arguments arg, const char *path, regex_t *re_list);

void print_match(regex_t *re, char *line, int line_count, arguments args,
                 const char *path);

#endif