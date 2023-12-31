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

#endif
