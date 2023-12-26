#ifndef GREP_H
#define GREP_H

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

#include "regex.h"

typedef struct arguments {
  int e, i, v, c, l, n, h, s, f, o;
  char* pattern;

  // int len_pattern;
  // int mem_pattern;

} arguments;

#endif
