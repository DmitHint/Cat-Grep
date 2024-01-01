#ifndef GREP_H
#define GREP_H

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>


#include "regex.h"
#include "string.h"


typedef int bool;

#define false 0
#define true 1


typedef struct arguments {
    int e, i, v, c, l, n, h, s, f, o;

    char **patterns;
    int patterns_len;

} arguments;

void pattern_add(arguments *opts, char *pattern);

void regs_from_file(arguments *opts, char *reg_path);

arguments parse_arguments(int argc, char **argv);

void output_line(char *line, int len);

void process_file(arguments arg, char *path, regex_t *re_list);

void print_match(regex_t *re, char *line);

#endif